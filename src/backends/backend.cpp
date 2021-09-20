#include <lua.hpp>
#include <iostream>
#include <unordered_map>

#include "backend.h"
#include "luah.h"
#include "global.h"

extern Backend* backend;
int luafunc_rule_new(lua_State* L) {
	const char* name = luaL_checkstring(L, 1);

	lerr(!lua_istable(L, 2), "Rule.new expects table as second argument\n");
	auto temp = Ltable_to_map<std::unordered_map<std::string, std::string>>(L, 2);
	
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

	std::vector<std::string> in = Ltable_to_vector(L, 3);
	auto opts = Ltable_to_map<std::unordered_map<std::string, std::string>>(L, 4);

	o->generate(out, in, opts);

	return 0;
}
