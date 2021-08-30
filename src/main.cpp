#include <iostream>
#include <lua.hpp>

#include "global.h"
#include "project.h"
#include "backends/ninja.h"

void define_functions(lua_State* L) {
	lua_register(L, "project", luafunc_project);

	// Rule
	// https://gist.github.com/zester/2438462
	luaL_Reg rule_regs[] = {
		{"new", luafunc_rule_new},
		{"generate", luafunc_rule_generate},
		{"__gc", luafunc_rule_destroy},
		{0, 0}
	};

	luaL_newmetatable(L, "Rule_meta");
	luaL_register(L, 0, rule_regs);

	lua_pushvalue(L, -1);
	lua_setfield(L, -1, "__index");
	lua_setglobal(L, "Rule");
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

	define_functions(L);

	int result = luaL_dofile(L, "tachyonfile.lua");
	Lcheck_err(result, L);

	lua_close(L);
	return 0;
}
