#include <lua.hpp>
#include <vector>
#include <string>

#pragma once

extern int r;

void err(const char* string, ...);
void lerr(bool iserr, const char* string, ...);

// vim: ft=cpp
