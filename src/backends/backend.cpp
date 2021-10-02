#include <lua.hpp>
#include <iostream>
#include <unordered_map>

#include "backend.h"
#include "luah.h"
#include "global.h"

extern Backend* backend;

static auto to_vec(sol::table const& t) {
    std::vector<std::string> result;
    for (auto const & e : t) {
        auto k = e.first.as<sol::optional<int>>();
        auto v = e.second.as<sol::optional<std::string>>();
        if (k && v) {
            result[k.value()] = v.value();
        }
    }
    return result;
}

static auto to_umap(sol::table const& t) {
	std::unordered_map<std::string, std::string> result;
    for (auto const & e : t) {
        auto k = e.first.as<sol::optional<std::string>>();
        auto v = e.second.as<sol::optional<std::string>>();
        if (k && v) {
            result[k.value()] = v.value();
        }
    }
    return result;
}

//
// LRule
//
LRule::LRule(std::string name, sol::table opts) {
	auto temp = opts.as<std::unordered_map<std::string, std::string>>();
	this->rule = backend->create_rule(name, temp);
}

void LRule::generate(std::string out, sol::table in, sol::table opts) {
	auto vec = in.as<std::vector<std::string>>();
	auto map = opts.as<std::unordered_map<std::string, std::string>>();
	this->rule->generate(out, vec, map);
}

int luafunc_rule_new(lua_State* L) {
	const char* name = luaL_checkstring(L, 1);

	lerr(!lua_istable(L, 2), "Rule.new expects table as second argument\n");
	auto temp = Ltable_to_map<std::unordered_map<std::string, std::string>>(L, 2);

	Rule** udata;
	udata = (Rule**)lua_newuserdata(L, sizeof(Rule**));
	*udata = backend->create_rule(name, temp);

	luaL_getmetatable(L, "Rule_meta");
	lua_setmetatable(L, -2);

	return 1;
}

int luafunc_rule_destroy(lua_State* L) {
	Rule* o = *(Rule**)lua_touserdata(L, 1);
	delete o;

	return 0;
}

int luafunc_rule_generate(lua_State* L) {
	Rule* o = *(Rule**)luaL_checkudata(L, 1, "Rule_meta");
	const char* out = luaL_checkstring(L, 2);

	std::vector<std::string> in = Ltable_to_vector(L, 3);
	auto opts = Ltable_to_map<std::unordered_map<std::string, std::string>>(L, 4);

	o->generate(out, in, opts);

	return 0;
}
