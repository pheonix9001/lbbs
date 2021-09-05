#include <string>
#include <lua.hpp>
#include <iostream>

#include "global.h"

namespace Project {
	std::string name;
	std::string desc;
	std::string lang;

	void isdefined() {
		lerr(name.size() == 0, "Function called without calling project()");
		lerr(lang.size() == 0, "Function called without calling project()");
	}
};

int luafunc_project(lua_State* L) {
	Project::name = luaL_checkstring(L, 1);
	Project::lang = luaL_checkstring(L, 2);

	lua_getfield(L, 3, "desc");
	Project::desc = luaL_checkstring(L, -1);
	lua_pop(L, 1);

	std::cerr << "-- Initializing project\n"
	"Project name: " << Project::name << "\n"
	<< "Language: " << Project::lang << "\n"
	<< "Description: " << Project::desc << std::endl;

	return 0;
}
