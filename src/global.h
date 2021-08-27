#include <lua.hpp>
#include <stdbool.h>

extern int r;

void err(const char* string, ...);
void lerr(bool iserr, const char* string, ...);

void Lprint_err(lua_State* state);
void Lcheck_err(int result, lua_State* state);

// vim: ft=cpp
