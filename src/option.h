#pragma once

#include <variant>
#include <map>
#include <string>
#include <lua.hpp>
#include <limits>
#include <sol/sol.hpp>

// generic option type
class OptionVal {
	private:
	public:
	sol::object data;

	OptionVal(): data(0) {
	}

	~OptionVal() {
	}
};

// void serialize_options();
// void deserialize_options();

sol::object luafunc_option(std::string name, sol::object def, std::optional<bool> ischecked);
sol::object luafunc_get_option(std::string name);

extern std::map<std::string, sol::object> options;
extern std::map<std::string, sol::object> cmd_options;
