#include <lua.hpp>
#include <string>
#include <vector>
#include <unordered_map>

class Rule {
public:
	virtual void generate(std::string out, std::vector<std::string>& in,
	std::unordered_map<std::string, std::string> opts)=0;
	virtual ~Rule() {}
};

class Backend {
public:
	virtual Rule* create_rule(std::string name,
	std::unordered_map<std::string, std::string>& extra_props)=0;
	virtual ~Backend() {}
};

int luafunc_rule_new(lua_State* L);
int luafunc_rule_generate(lua_State* L);
int luafunc_rule_destroy(lua_State* L);
