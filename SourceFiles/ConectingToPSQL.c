#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>
#include "FunctionPrototypes.h"

PGconn* conn;
PGresult* res;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Подключение к серверу
void ConectingToPSQL(void) {
	conn  = PQconnectdb("hostaddr = 10.0.2.15 dbname = messenger user = o4ina password = o4ina2000 port = 5432");
	if(PQstatus(conn) != CONNECTION_OK) {
		printf("Нет подключения к серверу\n");
	}
	//printf("%s\n", PQdb(conn)); 
    //printf("%s\n", PQhost(conn));
    //printf("%s\n\n", PQport(conn));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//регистрация
int PutRegData(char UserData[4][31], struct UserDataType *UserDataStruct) {  

	const char *UserDataPQ[5];

	for (int j = 0; j < 4; ++j) {
		UserDataPQ[j] = getParamsFromUserData(j, UserData);
	}
	UserDataPQ[4] = "NULL";

	res = PQexecParams(conn, "SELECT * from users WHERE email = $1", 1, NULL,UserDataPQ, NULL, NULL, 0);
	int rows = PQntuples(res);	
	if (rows) {
		fputs("\x1bm[31mТакой email уже существует\x1b[0m\n",stdout);
		return 0;
	}

	PQexecParams(conn, "INSERT INTO users(email,password,name,phone_number,status) VALUES ($1,$2,$3,$4,$5);",5,NULL,UserDataPQ,NULL,NULL,0);
	fputs("\x1b[32mРегистрация прошла успешно\x1b[0m\n",stdout);

	const char *UserDataPQLog[1];
	UserDataPQLog[0] = getParamsFromUserData(0, UserData);

	res = PQexecParams(conn, "SELECT id from users WHERE email = $1;", 1, NULL,UserDataPQLog, NULL, NULL, 0);
	strcpy(UserDataStruct->UserID, PQgetvalue(res, 0, 0));
	res = NULL;

	strcpy(UserDataStruct->Email, UserDataPQ[0]);
	strcpy(UserDataStruct->Name, UserDataPQ[2]);
	strcpy(UserDataStruct->Phone_number, UserDataPQ[3]);
	//strcpy(UserDataStruct->ChangeStatus, UserDataPQ[4]); ???????????????????
	fputs("\x1b[32mВход выполнен\x1b[0m\n",stdout);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Вход в учетную запись
int CheckLogData(char UserData[2][31], struct UserDataType *UserDataStruct) {
	const char *UserDataPQ[1];

	UserDataPQ[0] = getParamsFromUserData(0, UserData);

	res = PQexecParams(conn, "SELECT * from users WHERE email = $1;", 1, NULL,UserDataPQ, NULL, NULL, 0);
	int rows = PQntuples(res);	
	if (!rows) {
		fputs("\x1b[31mТакой учетной записи не существует\n\x1b[0m",stdout);
		
		return 0;
	}

	if (!strcmp(PQgetvalue(res, 0, 2), getParamsFromUserData(1, UserData))) {
		strcpy(UserDataStruct->UserID, PQgetvalue(res, 0, 0));
		strcpy(UserDataStruct->Email, PQgetvalue(res, 0, 1));
		strcpy(UserDataStruct->Name, PQgetvalue(res, 0, 3));
		strcpy(UserDataStruct->Phone_number, PQgetvalue(res, 0, 4));

		PQexecParams(conn, "update users set status = 'ChMesAndCor' where email = $1;", 1, NULL, UserDataPQ, NULL, NULL, 0); // Сомнительно

		//CheckChangeStatus(UserDataStruct);

		fputs("\x1b[32mВход выполнен\x1b[0m\n",stdout);
	} else {
		fputs("\x1b[31mНеверный пароль\x1b[0m\n",stdout);
	}
	res = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Проверка изменения данных
int CheckChangeStatus(struct UserDataType *UserDataStruct) {
	const char *UserDataPQ_1[1];
	UserDataPQ_1[0] = UserDataStruct->CorrespondenceID;

	const char *UserDataPQ_2[2];
	UserDataPQ_2[0] = UserDataStruct->UserID;
	UserDataPQ_2[1] = UserDataStruct->LastMessageID;

	const char *UserDataPQ_3[3];
	UserDataPQ_3[0] = UserDataStruct->CorrespondenceID;

	char *time;

	if (UserDataStruct->UserLocation == 0) {
		res = PQexecParams(conn, "SELECT missedmessage1_id FROM correspondence WHERE id = $1;", 1, NULL, UserDataPQ_1, NULL, NULL, 0);
		if (!strcmp(PQgetvalue(res, 0, 0),"f")) {
			return 0;
		} else {
			PQexecParams(conn,"UPDATE correspondence SET missedmessage1_id = 'f' WHERE id = $1;", 1, NULL, UserDataPQ_3, NULL, NULL, 0);
		}
	} else {
		res = PQexecParams(conn, "SELECT missedmessage2_id FROM correspondence WHERE id = $1;", 1, NULL, UserDataPQ_1, NULL, NULL, 0);
		if (!strcmp(PQgetvalue(res, 0, 0),"f")) {
			return 0;
		} else {
			PQexecParams(conn,"UPDATE correspondence SET missedmessage2_id = 'f' WHERE id = $1;", 1, NULL, UserDataPQ_3, NULL, NULL, 0);
		}
	}

	res = PQexecParams(conn, "SELECT id, text, date FROM message WHERE id > $2 AND user_id != $1;", 2, NULL, UserDataPQ_2, NULL, NULL, 0);  // Вывод сообщения, которое > ЭНного сообщения и UserID != Владелец сообщения
	if (!PQntuples(res)) {
		return 0;
	}

	for (int i = 0;i < PQntuples(res); ++i) {
		time = PQgetvalue(res, i, 2);
		*(strchr(time, '.')) = '\0';

		printf("\x1b[1m%s %s:\x1b[0m %s", time, UserDataStruct->Name2, PQgetvalue(res, i, 1));
		strcpy(UserDataStruct->LastMessageID, PQgetvalue(res, (PQntuples(res)-1), 0));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Вывод всех переписок
int WriteCorrespondence(struct UserDataType *UserDataStruct) {
	const char *UserID[1];
	UserID[0] = UserDataStruct->UserID;

	res = PQexecParams(conn,"SELECT u1.id, u1.name, correspondence.missedmessage1_id, u2.id, u2.name, correspondence.missedmessage2_id from correspondence left join users as u1 on correspondence.user1_id = u1.id left join users as u2 on correspondence.user2_id = u2.id WHERE (u1.id = $1 or u2.id = $1);", 1, NULL, UserID, NULL, NULL, 0);
	if (PQntuples(res) == 0) {
		fputs("\x1b[32mУ вас нет ни одного чата.\x1b[0m\n", stdout);     
		return 0;
	}

	register short int NumberChat = 0;
	for (register int i = 0; i < PQntuples(res); ++i) {
		if (!strcmp(PQgetvalue(res,i,0),UserDataStruct->UserID)) {
			if (!strcmp(PQgetvalue(res,i,2),"f")) {
				NumberChat++;
				printf("%hd - %s\n", NumberChat, PQgetvalue(res,i,4));
			}
		} else {
			if (!strcmp(PQgetvalue(res,i,5),"f")) {
				NumberChat++;
				printf("%hd - %s\n", NumberChat, PQgetvalue(res,i,1));
			}
		}
	}

	if (NumberChat == PQntuples(res)) {
		fputs("\x1b[32mУ вас нет новых сообщений.\x1b[0m\n", stdout);
		return 0;
	}

	fputs("\x1b[32mЕсть непрочитанные:\x1b[0m\n", stdout);

	for (register int i = 0; i < PQntuples(res); ++i) {
		if (!strcmp(PQgetvalue(res,i,0),UserDataStruct->UserID)) {
			if (!strcmp(PQgetvalue(res,i,2),"t")) {
				NumberChat++;
				printf("%hd - %s\n", NumberChat, PQgetvalue(res,i,4));
			}
		} else {
			if (!strcmp(PQgetvalue(res,i,5),"t")) {
				NumberChat++;
				printf("%hd - %s\n", NumberChat, PQgetvalue(res,i,1));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Вывод сообщений
int GetMessage(struct UserDataType *UserDataStruct, int NumberMessage) {
 	const char *UserDataPQ[1]; 
	UserDataPQ[0] = UserDataStruct->CorrespondenceID;

	int NumberLastNMessage = PQntuples(res) - NumberMessage;
	char* time;

	res = PQexecParams(conn, "SELECT id, text, date, user_id FROM message WHERE correspondence_id = $1;", 1, NULL, UserDataPQ, NULL, NULL, 0);
	if (!PQntuples(res)) {
		printf("Нет сообщений.\n");
		return 0;
	}

	if (NumberLastNMessage < 0) {
		NumberLastNMessage = 0;
	}


	for (int i = NumberLastNMessage; i < PQntuples(res); ++i) {
		time = PQgetvalue(res, i, 2);
		*(strchr(time, '.')) = '\0';

		printf("\x1b[1m%s %s:\x1b[0m %s", time, !strcmp(PQgetvalue(res, i, 3),UserDataStruct->UserID) ? UserDataStruct->Name : UserDataStruct->Name2, PQgetvalue(res, i, 1));
	}
	strcpy(UserDataStruct->LastMessageID, PQgetvalue(res, (PQntuples(res)-1), 0));
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Получение ID Пользователя_2, ID чата, имени 2 пользователя, а также расположение missedmessage основного пользователя в таблецe
int GetCorrespondenceID(char UserName[31], struct UserDataType *UserDataStruct) {
	const char *UserDataPQ_1[2];
	UserDataPQ_1[0] = UserName;
	UserDataPQ_1[1] = UserDataStruct->Name;

	const char *UserDataPQ_2[1];

	res = PQexecParams(conn,"SELECT correspondence.id, u1.id, u2.id from correspondence left join users as u1 on correspondence.user1_id = u1.id left join users as u2 on correspondence.user2_id = u2.id WHERE (u1.name = $1 AND u2.name = $2) OR (u1.name = $2 AND u2.name = $1);", 2, NULL, UserDataPQ_1, NULL, NULL, 0);
	if (PQntuples(res) == 0) {  
		return 1;
	}	

	UserDataPQ_2[0] = PQgetvalue(res, 0, 0); 

	if (!strcmp(PQgetvalue(res, 0, 1), UserDataStruct->UserID)) {
		UserDataStruct->UserLocation = 0;
		strcpy(UserDataStruct->User2ID, PQgetvalue(res, 0, 2));
		PQexecParams(conn,"UPDATE correspondence SET missedmessage1_id = 'f' WHERE id = $1;", 1, NULL, UserDataPQ_2, NULL, NULL, 0);
	} else {
		UserDataStruct->UserLocation = 1;
		strcpy(UserDataStruct->User2ID, PQgetvalue(res, 0, 1));
		PQexecParams(conn,"UPDATE correspondence SET missedmessage1_id = 'f' WHERE id = $1;", 1, NULL, UserDataPQ_2, NULL, NULL, 0);
	}
	strcpy(UserDataStruct->CorrespondenceID, PQgetvalue(res, 0, 0));
	strcpy(UserDataStruct->Name2,UserName);

	return 0;
} ;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Отправка сообщения
int PutMessage(char Message[401], struct UserDataType *UserDataStruct) {
	const char *UserDataPQ_1[3];
	UserDataPQ_1[0] = UserDataStruct->CorrespondenceID;
	UserDataPQ_1[1] = UserDataStruct->UserID; 
	UserDataPQ_1[2] = Message;

	const char *UserDataPQ_2[1];
	UserDataPQ_2[0] = UserDataStruct->CorrespondenceID;

	PQexecParams(conn,"INSERT INTO message(correspondence_id, user_id, text, date) VALUES ($1, $2, $3, clock_timestamp());", 3, NULL, UserDataPQ_1, NULL, NULL, 0);

	if (UserDataStruct->UserLocation) {
		PQexecParams(conn,"UPDATE correspondence SET missedmessage1_id = 't' WHERE id = $1;", 1, NULL, UserDataPQ_2, NULL, NULL, 0);
	} else {
		PQexecParams(conn,"UPDATE correspondence SET missedmessage2_id = 't' WHERE id = $1;", 1, NULL, UserDataPQ_2, NULL, NULL, 0);	
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Создание нового чата
int AddChat(char Email[31], struct UserDataType *UserDataStruct) {
	const char *UserDataPQ_1[1];
	UserDataPQ_1[0] = Email;

	const char *UserDataPQ_2[2];

	res = PQexecParams(conn, "SELECT id FROM users WHERE email = $1", 1, NULL, UserDataPQ_1, NULL, NULL, 0);
	if (!PQntuples(res)) {
		fputs("\x1b[32mПользователя с таким email-ом не существует.\x1b[0m\n", stdout);
		return 0;
	}
	UserDataPQ_2[0] = UserDataStruct->UserID;
	UserDataPQ_2[1] = PQgetvalue(res, 0, 0);

	res = PQexecParams(conn, "SELECT id FROM correspondence WHERE (user1_id = $1 AND user2_id = $2) OR (user1_id = $2 AND user2_id = $1)", 2, NULL, UserDataPQ_2, NULL, NULL, 0);
	if (PQntuples(res)) {
		fputs("\x1b[32mУ вас уже есть чат с этим пользователем.\x1b[0m\n", stdout);
		return 0;
	}

	PQexecParams(conn,"INSERT INTO correspondence(user1_id, user2_id, missedmessage1_id, missedmessage2_id) VALUES ($1, $2, 'f', 'f');", 2, NULL, UserDataPQ_2, NULL, NULL, 0);
	fputs("\x1b[32mЧат создан\x1b[0m\n", stdout);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Получение указателя на строку-литерал
inline char *getParamsFromUserData(short int UserDataIndex, char UserData[][31]) {
	char *Array = malloc(31);
	memset(Array,0,31);
	for (register int i = 0; i < 31; ++i) {
		Array[i] = UserData[UserDataIndex][i];
		if (UserData[UserDataIndex][i+1] == '\0') {
			Array[i+1] = '\0';
			break;
		}
	}
	return Array;
}