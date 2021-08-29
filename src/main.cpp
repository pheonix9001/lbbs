#include <iostream>
#include <lua.hpp>

#include "global.h"
#include "project.h"
#include "backends/ninja.h"

void define_functions(lua_State* L) {
	lua_register(L, "project", luafunc_project);
	lua_register(L, "custom_rule", luafunc_custom_rule);
}

Backend *backend;

int main (int argc, char *argv[]) {
	// initialize lua
	lua_State* L;
	L = luaL_newstate();
	luaL_openlibs(L);
	
	// initialize ninja backend
	// TODO: add command line option for backends
	Ninja temp{"build/build.ninja"};
	backend = &temp;

	int result = luaL_loadfile(L, "mesonbuild.lua");
	Lcheck_err(result, L);

	define_functions(L);

	result = lua_pcall(L, 0, LUA_MULTRET, 0);
	Lcheck_err(result, L);

	lua_close(L);
	return 0;
}
