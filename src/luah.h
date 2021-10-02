#include <lua.hpp>
#include <string>
#include <vector>

// lua functions
void Lcheck_err(int result, lua_State* state);
void Leval(lua_State* L, std::string str);
std::vector<std::string> Ltable_to_vector(lua_State* L, int index);

class LContext{
private:
	lua_State* L;
public:
	LContext(lua_State* _L): L(_L) {
	};
	~LContext();
};

template<typename T>
T Ltable_to_map(lua_State* L, int idx) {
	T temp;

	// push table and nil
	lua_pushvalue(L, idx);
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

	return temp;
}
