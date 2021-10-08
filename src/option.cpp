#include <iostream>
#include <fstream>
#include <map>
#include <limits>
#include <sol/sol.hpp>

#include "option.h"
#include "global.h"

std::map<std::string, sol::object> options;
std::map<std::string, sol::object> cmd_options;

//
// Lua bindings
//
sol::object luafunc_option(std::string name, sol::object def,
std::optional<bool> ischecked) {
	if(!ischecked) { ischecked = true; } // by default use type checking

	// 'default or cmd_options[name].data'
	sol::object actualval;
	if(cmd_options.find(name) == cmd_options.end()) {
		actualval = def;
	} else {
		actualval = cmd_options[name];
	}

	if(ischecked.value()) {
		if(actualval.get_type() != def.get_type()) {
			err("-- option %s has an invalid type\n");
		}
	}
	options[name] = actualval;

	return actualval;
}

sol::object luafunc_get_option(std::string name) {
	if(options.find(name) == options.end()) {
		err("-- get_option() called before option() for %s\n", name.c_str());
	} 

	return options[name];
}
