#include <iostream>
#include <fstream>
#include <map>

#include "option.h"
#include "global.h"

std::map<std::string, OptionVal> options;
std::map<std::string, OptionVal> tempoptions;

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

		getnextsub(start, end);
		std::string value = file.substr(start + 1, end);

		std::cout << klen << "|" << kkey << "|"
		<< vlen << "|" << vidx << "|" << value << std::endl;

		getnextsub(start, end);
	}
}

//
// OptionVal
//
OptionVal::OptionVal() {
	this->data = (void*)0;
}

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
