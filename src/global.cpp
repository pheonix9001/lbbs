#include <iostream>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "global.h"

int r;

void err(const char* string, ...) {
	va_list list;

	char* msg = strdup("\033[31m ERROR: \033[0m");
	strcat(msg, string);

	va_start(list, string);
	vfprintf(stderr, string, list);
	_Exit(-1);
}

void lerr(bool iserr, const char* string, ...) {
	if(iserr) {
		va_list list;
		va_start(list, string);

		char* msg = strdup("\033[31m ERROR:\033[0m ");
		strcat(msg, string);
		vfprintf(stderr, msg, list);
		_Exit(-1);
	}
}
