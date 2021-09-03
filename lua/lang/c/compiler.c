#define LUA_LIB
#include <lua.h>
#include <lauxlib.h>

#include "global.h"

void* c_compiler;
void* c_linker;

int executable(lua_State* L) {
	const char* outfile = luaL_checkstring(L, 1);

	printf("-- Generating executable %s\n", outfile);
	return 1;
}

static void gen_rules(lua_State* L) {
	const char* c_compiler_objs[][2] = {
		{"command", "gcc -c $cflags -o $out $in"},
		{"description", "Compiling $out..."},
		{0, 0}
	};
	c_compiler = Rule_new(L, "c_COMPILER", c_compiler_objs);

	const char* c_linker_objs[][2] = {
		{"command", "gcc $cflags $ldflags -o $out $in"},
		{"description", "Linking $out..."},
		{0, 0}
	};
	c_linker = Rule_new(L, "c_LINKER", c_linker_objs);
}

extern int luaopen_lang_c_compiler(lua_State *L) {
	luaL_Reg compilerlib[] = {
		{"executable", executable},
		{0, 0}
	};
	printf("-- Lua: loaded c compiler module\n");
	luaL_register(L, "lang/c/compiler", compilerlib);

	gen_rules(L);
	return 1;
}
