#include <lua.hpp>
#include <string>
#include <vector>

// lua functions
void Lcheck_err(int result, lua_State* state);
void Lsetpath(lua_State* L, const char* path);
void Lsetcpath(lua_State* L, const char* path);
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
