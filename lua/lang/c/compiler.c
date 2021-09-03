#define LUA_LIB
#include <lua.h>
#include <lauxlib.h>

void* c_compiler;
void* c_linker;

int executable(lua_State* L) {
	const char* outfile = luaL_checkstring(L, 1);

	printf("-- Generating executable %s\n", outfile);
	return 1;
}

static void* Rule_new(lua_State* L, const char* name, const char* objs[][2]) {
	// get len of objs
	int objs_len = 0;
	for(;;) {
		if(objs[objs_len][0] == 0  && objs[objs_len][1] == 0) {
			break;
		}
		objs_len++;
	}

	lua_getglobal(L, "Rule");
	lua_getfield(L, -1, "new");

	// setup args
	lua_pushstring(L, name);

	lua_createtable(L, 0, objs_len);
	for(int i = 0; i < objs_len; i++) {
		lua_pushstring(L, objs[i][1]);
		lua_setfield(L, -2, objs[i][0]);
	}

	// call function
	lua_call(L, 2, 1);

	void* ret = luaL_checkudata(L, -1, "Rule_meta");
	return ret;
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
