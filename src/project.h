#include <lua.hpp>
#include <string>
#include <vector>

namespace Project {
	extern std::string name;
	extern std::string desc;

	void isdefined();
};

int luafunc_project(lua_State* L);
