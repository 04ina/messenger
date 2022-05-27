struct UserDataType {
	char UserID[12];
	char Email[31];
	char Name[31];
	char Phone_number[31];

	char Name2[31];							// Сомнительно  
	char User2ID[12];						// ID 2 пользователя
	char CorrespondenceID[12];				// ID подключенного чата 
	char LastMessageID[12];					// ID последнего сообщения
	//const char *CorrespondenceID[256];
	//_Bool missedmessage[256];

	//Битовое поле
	//int ChangeStatus : 1 = 0;				// ?????????????
	int ChatConnection : 1; 			// Есть ли подключение к чату
	int UserLocation : 1; 					// Вспомогательная переменная для sql запроса    0 - left, 1 - right



};

int LoginAndRegistration(struct UserDataType *UserDataStruct);

// ConectingToPSQL.c
int PutRegData(char UserData[4][31], struct UserDataType *UserDataStruct);
int CheckLogData(char UserData[2][31], struct UserDataType *UserDataStruct);
int WriteCorrespondence(struct UserDataType *UserDataStruct);
int GetCorrespondenceID(char UserName[31], struct UserDataType *UserDataStruct);
int GetMessage(struct UserDataType *UserDataStruct, int NumberMessage);
int CheckChangeStatus(struct UserDataType *UserDataStruct);
int PutMessage(char Message[401], struct UserDataType *UserDataStruct);
int AddChat(char Email[31], struct UserDataType *UserDataStruct);

// AdditionalFunction.c
void ConectingToPSQL(void);
int CommandsViaAccount(struct UserDataType *UserDataStruct);
int JoinedTheChat(char UserName[31], struct UserDataType *UserDataStruct);
void* CheckChangeStatusEverySec(void* thread_data);

char *getParamsFromUserData(short int UserDataIndex, char UserData[][31]);

//void GetCorrespondence(struct UserDataType *UserDataStruct, const char *UserID[1]);




/*
	47 | o4ina@mail.ru | 1234     | o4ina   | 12552555     | ChMesAndCor

	54 | Anton@mail.ru | 123456   | Anton   | 5353535355   | ChMesAndCor

	 1 |       47 |       54 | f                 | t

	 от лица o4ina написать сообщение Антону

	

	 id                | integer                    |           | not null | nextval('message_id_seq'::regclass)
 correspondence_id | integer                    |           |          | 
 user_id           | integer                    |           |          | 
 text              | character varying(400)     |           |          | 
 date              | timestamp with time zone[] |           |          | 



	!log o4ina@mail.ru 1234
	!conmes Anton

	!log Anton@mail.ru 123456
	!conmes o4ina

	!log Grigory@gmail.ru 99944f
*/