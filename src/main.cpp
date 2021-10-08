#include <limits>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include <sol/sol.hpp>

#include "backends/ninja.h"
#include "global.h"
#include "option.h"

Backend* backend = 0;
std::string lbbsfile{"build.lua"};
char* cwd = new char[PATH_MAX];

void define_symbols(sol::state& S) {
	// set srcdir
	getcwd(cwd, 64);
	S["srcdir"] = cwd;

	S.new_usertype<LRule>("Rule",
		sol::constructors<LRule(), LRule(std::string, sol::table)>{},
		"generate", &LRule::generate
	);

	// Options
	S["option"] = &luafunc_option;
	S["get_option"] = &luafunc_get_option;
}

void cmd_line_parse(sol::state& S, int argc, char* const* argv) {
	// command line args
	for(;argc > 0;argc--, argv++) {
		if(argv[0][0] == '-') {
			int argv_idx = 1;
			char c = argv[0][argv_idx++];
			switch (c) {
				// general
				case 'f':
					lbbsfile = argv[1];
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

					auto code = "return " + value;
					cmd_options[key] = S.script(code);

					argc--;
					break;
				}
				default:
					err("Unknown option %c\n", c);
					break;
			}
		}
	}
}

inline void sol_panic(std::optional<std::string> msg) {
	if(msg) {
		std::cout << msg.value() << std::endl;
	}
}

int main(int argc, char *argv[]) {
	// initialize lua
	sol::state S;
	luaL_openlibs(S.lua_state());

	const std::string package_path = S["package"]["path"];
	S["package"]["path"] = package_path + ";/usr/lib/lbbs/?.lua";
	const std::string package_cpath = S["package"]["cpath"];
	S["package"]["cpath"] = package_cpath + ";/usr/lib/lbbs/?.so";

	// Initialization functions
	define_symbols(S);
	cmd_line_parse(S, argc, argv);

	if(backend == 0) {
		backend = new Ninja{"build/build.ninja"};
	}

	// generate regenerate Rule
	[](){
		std::map<std::string, std::string> opts = {
			{"command", (std::string)"cd " + cwd + " && lbbs"},
			{"description", "Regenerating $out..."},
			{"generator", "1"}
		};
		auto regenerate_rule = backend->create_rule("regenerate", opts);

		opts.clear();
		std::vector<std::string> in = { "../" + lbbsfile };
		regenerate_rule->generate("build.ninja", in, opts);
	}();

	S.script_file(lbbsfile);

	delete backend;
	_exit(0);
	return 0;
}
