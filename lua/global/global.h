#include <lua.h>

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
