#include <limits>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include <sol/sol.hpp>

#include "backends/ninja.h"
#include "global.h"
#include "luah.h"
#include "option.h"

Backend* backend = 0;
// char* lbbsfile = (char*)"build.lua";
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
	S.set_function("option",&luafunc_option);
	S.set_function("get_option",&luafunc_get_option);
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

					cmd_options[key].data = value;
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
	S.open_libraries(sol::lib::package,
	sol::lib::base,
	sol::lib::os,
	sol::lib::jit,
	sol::lib::io,
	sol::lib::math, sol::lib::string, sol::lib::table);

	const std::string package_path = S["package"]["path"];
	S["package"]["path"] = package_path + ";/usr/lib/lbbs/?.lua";
	const std::string package_cpath = S["package"]["cpath"];
	S["package"]["cpath"] = package_cpath + ";/usr/lib/lbbs/?.so";

	// Initialization functions
	cmd_line_parse(argc, argv);
	define_symbols(S);

	if(backend == 0) {
		backend = new Ninja{"build/build.ninja"};
	}

	try {
		std::map<std::string, std::string> opts = {
			{"command", (std::string)"cd " + cwd + " && lbbs"},
			{"description", "Regenerating $out..."},
			{"generator", "1"}
		};
		auto regenerate_rule = backend->create_rule("regenerate", opts);

		opts.clear();
		std::vector<std::string> in = { "../" + lbbsfile };
		regenerate_rule->generate("build.ninja", in, opts);

		S.script_file(lbbsfile);

		serialize_options();
	} catch (sol::error e) {
		std::cout << e.what() << std::endl;
	}

	delete backend;
	return 0;
}
