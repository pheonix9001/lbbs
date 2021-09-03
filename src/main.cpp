#include <cstring>
#include <iostream>
#include <lua.hpp>

#include "global.h"
#include "project.h"
#include "backends/ninja.h"

Backend* backend = 0;
char* tachyonfile = (char*)"tachyonfile.lua";

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

void cmd_line_parse(int argc, char* const* argv) {
	// command line args
	for(;argc > 0;argc--, argv++) {
		if(argv[0][0] == '-') {
			int argv_idx = 1;
			char c = argv[0][argv_idx++];
			switch (c) {
				case 'f':
					tachyonfile = argv[1];
					argc--;
					break;
				case 'b': {
					char* backendname = argv[1];
					if(strcmp(backendname, "ninja") == 0) {
						backend = new Ninja{"build/build.ninja"};
						std::cerr << "-- Set backend to ninja" << std::endl;
					}
					argc--;
					break;
				}
				default:
					err("Unknown option %c", c);
					break;
			}
		}
	}
}

int main (int argc, char *argv[]) {
	// initialize lua
	lua_State* L;
	L = luaL_newstate();

	// set library path
	luaL_openlibs(L);
	Lsetpath(L, "/usr/lib/tachyon/?.so");

	cmd_line_parse(argc, argv);
	if(backend == 0) {
		backend = new Ninja{"build/build.ninja"};
	}

	define_functions(L);

	int result = luaL_dofile(L, tachyonfile);
	Lcheck_err(result, L);

	lua_close(L);
	delete backend;
	return 0;
}
