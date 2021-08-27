#include <iostream>
#include <lua.hpp>

lua_State* L;

int main (int argc, char *argv[]) {
	L = luaL_newstate();

	lua_close(L);
	return 0;
}
