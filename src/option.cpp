#include "option.h"
#include "global.h"

//
// OptionVal
//
void OptionVal::getfromidx(lua_State* L, int idx) {
	int type = lua_type(L, idx);

	switch(type) {
		case LUA_TNUMBER:
			this->data = (int)lua_tointeger(L, idx);
			break;
		case LUA_TSTRING:
			this->data = lua_tostring(L, idx);
			break;
		default:
			err("Type convertion for type not implemented");
			break;
	}
}
