#include <lua.hpp>
#include <string>
#include <vector>
#include <map>

#include <limits>
#include <sol/sol.hpp>

class Rule {
public:
	virtual void generate(std::string out, std::vector<std::string>& in,
	std::map<std::string, std::string> opts)=0;
	virtual ~Rule() {}
};

class Backend {
public:
	virtual Rule* create_rule(std::string name,
	std::map<std::string, std::string>& extra_props)=0;
	virtual ~Backend() {}
};

// Lua version of Rule
class LRule{
private:
	Rule* rule;
public:
	LRule(std::string name, sol::table opts);
	LRule() {};
	~LRule() { delete rule; };

	void generate(std::string out, sol::table in,
	sol::table opts);
};
