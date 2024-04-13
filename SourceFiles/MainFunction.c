#include <stdio.h>
#include <stdlib.h>
#include "FunctionPrototypes.h"
#include <stdbool.h>

void main(void) {
	struct UserDataType UserDataStruct;
	UserDataStruct.UserID[0] = '\0';

	ConectingToPSQL();

	fputs("\x1b[32m!help\x1b[0m\n", stdout);

	while (UserDataStruct.UserID[0] == '\0') {
		if (LoginAndRegistration(&UserDataStruct)) {
			exit(1);
		}
	}
	while (!CommandsViaAccount(&UserDataStruct)) {}

	 




}
