#include <iostream>
#include <lua.hpp>

#include "global.h"

lua_State* L;

int main (int argc, char *argv[]) {
	L = luaL_newstate();
	luaL_openlibs(L);

	int result = luaL_loadfile(L, "mesonbuild.lua");
	Lcheck_err(result, L);

	result = lua_pcall(L, 0, LUA_MULTRET, 0);
	Lcheck_err(result, L);

	lua_close(L);
	return 0;
}
