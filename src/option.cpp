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
		std::string value = std::get<std::string>(opt.second.data);

		fs << key.length() << "\033" << key << "\033" << 
		value.length() << "\033" << opt.second.data.index() << "\033" << value  << "\033";
	}
	fs.flush();
}

void deserialize_options() {
	std::ifstream fs{"build/tachyon_opts.txt"};
	std::string file;

	fs >> file;

	auto getnextsub = [&file](int& start, int& end) {
		start = end;
		end = file.find("\033", start + 1);
	};

	int start = 0;
	int end = file.find("\033");

	for (;(std::string::size_type)end != std::string::npos;) {
		int klen = std::stoi(file.substr(start, end));

		start = end;
		end += klen + 1;
		std::string kkey = file.substr(start + 1, end);

		getnextsub(start, end);
		int vlen = std::stoi(file.substr(start + 1, end));

		getnextsub(start, end);
		int vidx = std::stoi(file.substr(start + 1, end));

		start = end;
		end += vlen + 1;
		std::string value = file.substr(start + 1, end);

		switch (vidx) {
			case 0:
				cmd_options[kkey].data = std::stoi(value);
				break;
			case 1:
				cmd_options[kkey].data = value;
				break;
			default:
				err("-- Option parsing error\n");
				break;
		}
		getnextsub(start, end);
	}
}

//
// OptionVal
//
void OptionVal::getfromidx(lua_State* L, int idx) {
	int type = lua_type(L, idx);

	switch(type) {
		case LUA_TNUMBER:
			this->data = (int)lua_tointeger(L, idx);
			break;
		case LUA_TSTRING:
			this->data = lua_tostring(L, idx);
			break;
		default:
			err("Type convertion for type not implemented");
			break;
	}
}

//
// Lua bindings
//
int luafunc_option(lua_State* L) {
	const char* name = luaL_checkstring(L, 1);
	int type = lua_type(L, 2);
	bool ischecked = lua_toboolean(L, 3);

	switch (type) {
		case LUA_TNUMBER:
			if(cmd_options.find(name) == cmd_options.end()) {
				options[name].data = (int)lua_tointeger(L, 2);
				return 0;
			}
			if(ischecked && cmd_options[name].data.index() != 0) {
				err("Type given to option() is invalid.\n");
			}
			options[name] = cmd_options[name];
			break;
		case LUA_TSTRING:
			if(cmd_options.find(name) == cmd_options.end()) {
				options[name].data = lua_tostring(L, 2);
				return 0;
			}
			if(ischecked && cmd_options[name].data.index() != 1) {
				err("Type given to option() is invalid.\n");
			}
			options[name] = cmd_options[name];
			break;
		default:

			break;
	}

	return 0;
}
