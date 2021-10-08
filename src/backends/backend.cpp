#include <lua.hpp>
#include <iostream>
#include <map>

#include "backend.h"
#include "global.h"

extern Backend* backend;

//
// LRule
//
LRule::LRule(std::string name, sol::table opts) {
	auto temp = opts.as<std::map<std::string, std::string>>();
	this->rule = backend->create_rule(name, temp);
}

void LRule::generate(std::string out, sol::table in, sol::table opts) {
	auto vec = in.as<std::vector<std::string>>();
	auto map = opts.as<std::map<std::string, std::string>>();
	this->rule->generate(out, vec, map);
}
