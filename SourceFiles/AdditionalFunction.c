#include <stdio.h>
#include <stdlib.h>
#include "FunctionPrototypes.h"
#include <string.h>
#include <pthread.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int LoginAndRegistration(struct UserDataType *UserDataStruct) {
	char command[150];
	char choice[6];
	short int StartArguments = 0;

	fgets(command,150,stdin);
	for (; command[StartArguments] != '\n' && command[StartArguments] != ' '; ++StartArguments) {
		choice[StartArguments] = command[StartArguments];

	}
	choice[StartArguments] = '\0';
	StartArguments++;

	if (!strcmp(choice,"!help")) { 			// !help 
		fputs("\x1b[32m!reg Email Пароль Имя НомерТефона - Регистрация новой учетной записи.\n!log Email Пароль - Вход в учетную запись.\n!exit - Выход.\n\x1b[0m", stdout);
		return 0;   // !help
	} else if (!strcmp(choice,"!reg")){     // !reg  
		if (strlen(command) < 6) {
			fputs("Ошибка в синтаксисе\n", stdout);
			exit(1);
		}
		char UserData[4][31]; // 0 email  1 password  2 name  3 phone_number  

		for (int i = StartArguments, j = 0, NumArgument = 0; i < strlen(command) && NumArgument < 4; ++i) {
			if ( (j == 0 && command[i] == ' ') || (NumArgument !=3 && command[i] == '\n') ) {
				fputs("Ошибка в синтаксисе\n", stdout);
				exit(1);
			}

			if (command[i] == ' ' || command[i] == '\n') {
				UserData[NumArgument][j] = '\0';
				j = 0;
				NumArgument++;
				continue;
			}
			if (j == 30) {
				switch(NumArgument) {
					case 0:
						fputs("Слишком длинный email\n", stdout);
					break;
					case 1:
						fputs("Слишком длинный пароль\n", stdout);
					break;
					case 2:
						fputs("Слишком длинное имя\n", stdout);
					break;
					case 3:
						fputs("Слишком длинный номер телефона\n", stdout);
					break;
				}
				exit(1);
			}

			UserData[NumArgument][j] = command[i];
			j++;
		}
		PutRegData(UserData, UserDataStruct);
		return 0;  // !reg 
	} else if (!strcmp(choice,"!log")){     // !log  
		if (strlen(command) < 6) {
			fputs("Ошибка в синтаксисе\n", stdout);
			return 0;
		}
		char UserData[2][31];

		for (int i = StartArguments, j = 0, NumArgument = 0; i < strlen(command) && NumArgument < 2; ++i) {
			if ( (j == 0 && command[i] == ' ') || (NumArgument !=1 && command[i] == '\n' ) ) {
				fputs("Ошибка в синтаксисе\n", stdout);
				return 0;
			}

			if (command[i] == ' ' || command[i] == '\n') {
				UserData[NumArgument][j] = '\0';
				j = 0;
				NumArgument++;
				continue;
			}
			if (j == 30) {
				switch(NumArgument) {
					case 0:
						fputs("Слишком длинный email\n", stdout);
					break;
					case 1:
						fputs("Слишком длинный пароль\n", stdout);
					break;
				}
				exit(1);
			}

			UserData[NumArgument][j] = command[i];
			j++;
		}
		CheckLogData(UserData, UserDataStruct);
		return 0;	 	
	} else if(!strcmp(choice,"!exit")){    // !exit 
		return 1;
	} else {
		fputs("Ошибка в синтаксисе\n", stdout);
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CommandsViaAccount(struct UserDataType *UserDataStruct) {

	char command[150];
	char choice[8];
	short int StartArguments = 0;

	fgets(command,150,stdin);
	for (; command[StartArguments] != '\n' && command[StartArguments] != ' '; ++StartArguments) {
		choice[StartArguments] = command[StartArguments];

	}
	choice[StartArguments] = '\0';
	StartArguments++;

	if (!strcmp(choice,"!help")) {          // !help
		fputs("\x1b[32m!getcor - вывод всех чатов\n!conmes ИмяПользователя - подключение к чату\n!addcht Email - создание нового чата\n!exit - выход\n\x1b[0m", stdout);
		return 0;
	} else if (!strcmp(choice,"!getcor")){ 	// !getcor
		WriteCorrespondence(UserDataStruct); // Сомнительно !getcor -> !getchat
		return 0;
	} else if (!strcmp(choice,"!conmes")){  // !conmes
		if (strlen(command) < 8) {
			fputs("Ошибка в синтаксисе\n", stdout);
			return 0;
		}
		char UserData[31];

		for (int i = StartArguments, j = 0, NumArgument = 0; i < strlen(command) && NumArgument < 1; ++i) { // Сомнительно (Убрать NumArgument)
			
			if ( (j == 0 && command[i] == ' ') || (NumArgument !=0 && command[i] == '\n' ) ) {
				fputs("Ошибка в синтаксисе\n", stdout);
				return 0;
			}

			if (command[i] == ' ' || command[i] == '\n') {
				UserData[j] = '\0';
				j = 0;
				NumArgument++;
				continue;
			}

			if (j == 30) { // Сомнительно
				fputs("Слишком длинное имя пользователя\n", stdout);
				return 0;
			}

			UserData[j] = command[i];
			j++;
		}

		JoinedTheChat(UserData, UserDataStruct);

		return 0;													// Сомнительно 
	} else if (!strcmp(choice,"!addcht")){  // !addcht
		if (strlen(command) < 8) {
			fputs("Ошибка в синтаксисе\n", stdout);
			return 0;
		}
		char UserData[31];

		for (int i = StartArguments, j = 0, NumArgument = 0; i < strlen(command) && NumArgument < 1; ++i) { // Сомнительно (Убрать NumArgument)
			
			if ( (j == 0 && command[i] == ' ') || (NumArgument !=0 && command[i] == '\n' ) ) {
				fputs("Ошибка в синтаксисе\n", stdout);
				return 0;
			}

			if (command[i] == ' ' || command[i] == '\n') {
				UserData[j] = '\0';
				j = 0;
				NumArgument++;
				continue;
			}

			if (j == 30) { // Сомнительно
				fputs("Слишком длинный email", stdout);
				return 0;
			}

			UserData[j] = command[i];
			j++;
		}

		AddChat(UserData, UserDataStruct);

		return 0;
	} else if (!strcmp(choice,"!exit")){    // !exit
		return 2;	// Сомнительно
	} else {
		fputs("Ошибка в синтаксисе\n", stdout);
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int JoinedTheChat(char UserName[31], struct UserDataType *UserDataStruct) {

	if (GetCorrespondenceID(UserName, UserDataStruct)) { // Получение id чата     буул 0 - слева \ 1 - справа changeStatuc чтоб чекченжестатусеверисек понал, где именно находится пропущенное сообщение активного пользователя
		fputs("Такого пользователя не существует\n", stdout);
		return 0;
	}
	fprintf(stdout, "\x1b[32mВы подключены к чату %s.\x1b[0m\n", UserName);

	GetMessage(UserDataStruct, 30); /////////////////////////////////////////// Вывод последних 30 сообщений      самое последнее сообщение пометить

	
	UserDataStruct->ChatConnection = 1;

	pthread_t thread; // идентификатор потока
	void* thread_data = UserDataStruct;
	//UserDataStruct->UserDataStruct = 1;
	pthread_create(&thread, NULL, CheckChangeStatusEverySec, thread_data);

	while (1) { // 1 or Nothing?
	
		char command[401];
		char choice[8];
		short int StartArguments = 0;

		fgets(command,401,stdin);

		if (command[0] == '!') {
			for (; command[StartArguments] != '\n' && command[StartArguments] != ' '; ++StartArguments) {
				choice[StartArguments] = command[StartArguments];
			}
			choice[StartArguments] = '\0';
			StartArguments++;

			if (!strcmp(choice,"!help")) {            //!help
				fputs("\x1b[32m!getmes Кол-воСообщений - вывод более старых сообщений\n!exit - выход\n\x1b[0m", stdout);
				//
			} else if (!strcmp(choice,"!exit")) {     //!exit
				UserDataStruct->ChatConnection = 0;
				fputs("\x1b[32mВы вышли из чата.\x1b[0m\n", stdout);
				return 1;
			} else if (!strcmp(choice,"!getmes")) {   //!getmes
				//
				//
				//
			} else {
				fputs("Ошибка в синтаксисе\n", stdout);
			}
			continue;	
		} else {
			PutMessage(command, UserDataStruct);
		}
		


	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* CheckChangeStatusEverySec(void* thread_data){
	struct UserDataType *UserDataStruct = (struct UserDataType *) thread_data;

	int value = clock()/CLOCKS_PER_SEC;
	while (UserDataStruct->ChatConnection) {
		if (clock()/CLOCKS_PER_SEC >= value) {
			
			CheckChangeStatus(UserDataStruct);
			
			value++;
		}
	}
	pthread_exit(0);

}

