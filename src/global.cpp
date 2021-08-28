#include <iostream>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <lua.hpp>
#include <string.h>

int r;

void err(const char* string, ...) {
	va_list list;

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

void Lprint_err(lua_State* state) {
	const char* msg = lua_tostring(state, -1);
	std::cerr << "Lua error: " <<  msg << std::endl;
	lua_pop(state, 1);
}

void Lcheck_err(int result, lua_State* state) {
	if(result != 0) {
		Lprint_err(state);
	}
}
