#include <iostream>
#include <fstream>
#include <map>

#include "option.h"
#include "global.h"

std::map<std::string, OptionVal> options;
std::map<std::string, OptionVal> cmd_options;

void serialize_options() {
	std::ofstream fs{"build/tachyon_opts.txt"};

	for(auto opt: options) {
		std::string key = opt.first;
		std::string value = opt.second.data;

		fs << key.length() << "\033" << key << "\033" << 
		value.length() << "\033" << value  << "\033";
	}
	fs.flush();
}

void deserialize_options() {
	std::ifstream fs{"build/tachyon_opts.txt"};
	std::string file;

	fs >> file;

	auto getnextsub = [&file](int& start, int& end) {
		start = end + 1;
		end = file.find("\033", start);
	};

	int start = 0;
	int end = file.find("\033");

	for (;(std::string::size_type)end != std::string::npos;) {
		int klen = std::stoi(file.substr(start, end));

		start = end + 1;
		end += klen + 1;
		std::string kkey = file.substr(start, end - 2);

		getnextsub(start, end);
		int vlen = std::stoi(file.substr(start, end - 2));

		start = end + 1;
		end += vlen + 1;
		std::string value = file.substr(start, end);

		getnextsub(start, end);
		cmd_options[kkey].data = value;
	}
}


//
// Lua bindings
//
int luafunc_option(lua_State* L) {
	const char* name = luaL_checkstring(L, 1);
	int type = lua_type(L, 2);
	std::string def = luaL_checkstring(L, 2);
	bool ischecked = lua_toboolean(L, 3);

	std::string actualval;
	if(cmd_options.find(name) == cmd_options.end()) {
		actualval = def;
	} else {
		actualval = cmd_options[name].data;
	}

	options[name].data = actualval;
	actualval = "return " + actualval;
	Leval(L, actualval);
	if(ischecked) {
		int vtype = lua_type(L, -1);
		if(vtype != type) {
			err("-- option %s has an invalid type\n");
		}
	}

	return 1;
}

int luafunc_get_option(lua_State* L) {
	const char* name = luaL_checkstring(L, 1);

	if(options.find(name) == options.end()) {
		err("-- get_option() called before option() for %s\n", name);
	} 

	Leval(L, "return " + options[name].data);
	return 1;
}
