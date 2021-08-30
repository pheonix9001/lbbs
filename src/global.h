#include <lua.hpp>
#include <stdbool.h>
#include <vector>
#include <string>

extern int r;

void err(const char* string, ...);
void lerr(bool iserr, const char* string, ...);

// lua functions
void Lprint_err(lua_State* state);
void Lcheck_err(int result, lua_State* state);
std::vector<std::string> Ltable_to_vector(lua_State* L, int index);

// vim: ft=cpp
