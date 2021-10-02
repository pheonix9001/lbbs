#include <lua.hpp>
#include <string>
#include <vector>
#include <iostream>

void Lcheck_err(int result, lua_State* state) {
	if(result != 0) {
	const char* msg = lua_tostring(state, -1);
	std::cerr << "Lua error: " <<  msg << std::endl;
	lua_pop(state, 1);
	}
}

void Leval(lua_State* L, std::string str) {
	lua_getglobal(L, "loadstring");
	lua_pushstring(L, str.c_str());

	lua_call(L, 1, 1);
	lua_call(L, 0, 1);
}

std::vector<std::string> Ltable_to_vector(lua_State* L, int index) {
	std::vector<std::string> temp;

	lua_pushvalue(L, index);
	int len = luaL_getn(L, -1);
	temp.resize(len);

	for (int i = 0; i < len; i++) {
		lua_pushinteger(L, i + 1);
		lua_gettable(L, -2);

		const char* str = luaL_checkstring(L, -1);
		temp[i] = str;

		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	return temp;
}
