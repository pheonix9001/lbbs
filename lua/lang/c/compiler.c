#define LUA_LIB
#include <lua.h>
#include <lauxlib.h>

int executable(lua_State* L) {
	const char* outfile = luaL_checkstring(L, 1);
	printf("-- Generating executable %s\n", outfile);
	return 1;
}

extern int luaopen_lang_c_compiler(lua_State *L) {
	luaL_Reg compilerlib[] = {
		{"executable", executable},
		{0, 0}
	};
	luaL_register(L, "lang/c/compiler", compilerlib);
	printf("-- Lua: loaded c compiler module\n");
	return 1;
}
