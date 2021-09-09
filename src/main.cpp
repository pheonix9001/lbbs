#include <cstring>
#include <iostream>
#include <lua.hpp>

#include "project.h"
#include "backends/ninja.h"
#include "global.h"
#include "option.h"

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

	// Options
	lua_register(L, "option", luafunc_option);
}

void cmd_line_parse(int argc, char* const* argv) {
	deserialize_options();

	// command line args
	for(;argc > 0;argc--, argv++) {
		if(argv[0][0] == '-') {
			int argv_idx = 1;
			char c = argv[0][argv_idx++];
			switch (c) {
				// general
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

				// options
				case 'd': {
					std::string str;
					str.assign(argv[1]);

					auto split = str.find("=");
					auto end = str.length();

					std::string key = str.substr(0, split);
					std::string value = str.substr(split + 1, end);

					if(value[0] == '\'' || value[0] == '\"') {
						auto actualval = value.substr(1, value.length());

						std::cout << "-- Setting string option " << key << "=" << actualval << std::endl;
						cmd_options[key].data = actualval;
					} else {
						try {
							std::cout << "-- Setting integer option " << key << " = " << value << std::endl;
							cmd_options[key].data = std::stoi(value);
						} catch(std::invalid_argument) {
							err("-- Use \'str or \"str when passing a string\n");
						}
					}
					break;
				}
				default:
					err("Unknown option %c\n", c);
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

	// Initialization functions
	cmd_line_parse(argc, argv);
	define_functions(L);

	if(backend == 0) {
		backend = new Ninja{"build/build.ninja"};
	}

	int result = luaL_dofile(L, tachyonfile);
	Lcheck_err(result, L);

	serialize_options();

	lua_close(L);
	delete backend;
	return 0;
}
