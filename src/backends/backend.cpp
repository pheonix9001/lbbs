#include <lua.hpp>
#include <iostream>

#include "backend.h"

extern Backend* backend;

int luafunc_custom_rule(lua_State* L) {
	const char* name = lua_tostring(L, 1);

	std::map<std::string, std::string> temp;
	
	// push table and nil
	lua_pushvalue(L, 2);
	lua_pushvalue(L, -2);
	lua_pushnil(L);

	while(lua_next(L, -2)) {
		lua_pushvalue(L, -2);

		// get key and value
		const char* key = lua_tostring(L, -1);
		const char* value = lua_tostring(L, -2);
		temp[key] = value;

		lua_pop(L, 2);
	}
	lua_pop(L, 1);

	backend->create_rule(name, temp);

	return 0;
}
