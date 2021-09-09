#pragma once

#include <variant>
#include <map>
#include <string>
#include <lua.hpp>

// generic option type
class OptionVal {
	private:
	public:
	std::variant<int, std::string> data;

	void getfromidx(lua_State* L, int idx);
};

void serialize_options();
void deserialize_options();

extern std::map<std::string, OptionVal> options;
extern std::map<std::string, OptionVal> cmd_options;
