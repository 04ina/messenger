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
