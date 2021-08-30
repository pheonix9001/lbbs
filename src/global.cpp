#include <iostream>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <lua.hpp>
#include <string.h>
#include <vector>

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

std::vector<std::string> Ltable_to_vector(lua_State* L, int index) {
	std::vector<std::string> temp;

	lua_pushvalue(L, index);
	int len = luaL_getn(L, -1);
	temp.resize(len);

	for (int i = 0; i < len; i++) {
		lua_pushinteger(L, i + 1);
		lua_gettable(L, -2);

		const char* str = luaL_checkstring(L, -1);
		temp[i] = str;
	
		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	return temp;
}
