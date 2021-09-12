#pragma once

#include <variant>
#include <map>
#include <string>
#include <lua.hpp>

// generic option type
class OptionVal {
	private:
	public:
	std::string data;
};

void serialize_options();
void deserialize_options();

int luafunc_option(lua_State* L);
int luafunc_get_option(lua_State* L);

extern std::map<std::string, OptionVal> options;
extern std::map<std::string, OptionVal> cmd_options;
