#include <lua.hpp>
#include <iostream>
#include <unordered_map>

#include "backend.h"
#include "global.h"

extern Backend* backend;
int luafunc_rule_new(lua_State* L) {
	const char* name = luaL_checkstring(L, 1);

	std::unordered_map<std::string, std::string> temp;
	
	// push table and nil
	lua_pushvalue(L, 2);
	lua_pushnil(L);

	// get rule from backend
	while(lua_next(L, -2)) {
		lua_pushvalue(L, -2);

		// get key and value
		const char* key = lua_tostring(L, -1);
		const char* value = lua_tostring(L, -2);
		temp[key] = value;

		lua_pop(L, 2);
	}
	lua_pop(L, 1);

	Rule** udata;
	udata = (Rule**)lua_newuserdata(L, sizeof(Rule**));
	*udata = backend->create_rule(name, temp);

	luaL_getmetatable(L, "Rule_meta");
	lua_setmetatable(L, -2);

	return 1;
}

int luafunc_rule_destroy(lua_State* L) {
	Rule* o = *(Rule**)lua_touserdata(L, 1);
	delete o;

	return 0;
}

int luafunc_rule_generate(lua_State* L) {
	Rule* o = *(Rule**)luaL_checkudata(L, 1, "Rule_meta");
	const char* out = luaL_checkstring(L, 2);

	std::vector<std::string> in;
	in = Ltable_to_vector(L, 3);

	o->generate(out, in);

	return 0;
}
