#include <stdio.h>
#include <stdlib.h>
#include "FunctionPrototypes.h"
#include <stdbool.h>

void main(void) {
	struct UserDataType UserDataStruct;
	UserDataStruct.UserID[0] = '\0';
	//pthread_t thread; // идентификатор потока
	//void* thread_data = &UserDataStruct;

	ConectingToPSQL();

	fputs("\x1b[32m!help\x1b[0m\n", stdout);
	//LoginAndRegistration(&UserDataStruct);	

	while (UserDataStruct.UserID[0] == '\0') {
		if (LoginAndRegistration(&UserDataStruct)) {
			exit(1);
		}
	}
	while (!CommandsViaAccount(&UserDataStruct)) {}

	 

	//pthread_create(&thread, NULL, threadFunc, thread_data); -----------------------------------------------



	/*while { 
		if (ChatConnection) {
			
			
		} else {

			ChatConnection = CommandsViaAccount();
			if (ChatConnection == 2) {
				exit(1);
			}
		}		
	}*/






/*
	while {
		if (UserDataStruct.UserID[0] == '\0') {
			if (LoginAndRegistration(&UserDataStruct)) {
				exit(1);
			}
			//printf("gfgdgdf\n");
		} else {
			CheckChangeStatus(&UserDataStruct);

			if (!ChatConnection) {
				ChatConnection = CommandsViaAccount();
			} else {

			}

			//CommandsViaAccount();


		}
	CheckChangeStatus(&UserDataStruct);*/
	//}


	//printf("%s %s %s %s\n", UserDataStruct.UserID, UserDataStruct.Email, UserDataStruct.Name, UserDataStruct.Phone_number);


	//Начальное меню  
		//Регистрация
		//Вход в аккаунт 
	//!help
		//Открыть контакты
			//Открыть переписку (открываетются последние 30 сообщений)
				//открыть последние n сообщений
				// !mg Текст - отправить сообщение
			//Информация о пользователе

		//Найти пользователя по имени
			//открыть переписку
			//Информация о пользователе
			//добавить в контакты



		//Открыть переписки 






}