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

// https://stackoverflow.com/questions/4125971/setting-the-global-lua-path-variable-from-c-c
void Lsetpath(lua_State* L, const char* path) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "path"); // get field "path" from table at top of stack
	std::string cur_path = lua_tostring(L, -1); // grab path string from top of stack
	cur_path.append(";"); // do your path magic here
	cur_path.append(path);
	lua_pop(L, 1); // get rid of the string on the stack we just pushed on line 5
	lua_pushstring(L, cur_path.c_str()); // push the new one
	lua_setfield(L, -2, "path"); // set the field "path" in table at -2 with value at top of stack
	lua_pop(L, 1); // get rid of package table from top of stack
}

void Lsetcpath(lua_State* L, const char* path) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "cpath"); // get field "cpath" from table at top of stack (-1)
	std::string cur_path = lua_tostring(L, -1); // grab path string from top of stack
	cur_path.append(";"); // do your path magic here
	cur_path.append(path);
	lua_pop(L, 1); // get rid of the string on the stack we just pushed on line 5
	lua_pushstring(L, cur_path.c_str()); // push the new one
	lua_setfield(L, -2, "cpath"); // set the field "path" in table at -2 with value at top of stack
	lua_pop(L, 1); // get rid of package table from top of stack
}