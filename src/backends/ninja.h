#include <fstream>
#include <unordered_map>

#include "backends/backend.h"

class Ninja;

class NinjaRule: protected Rule {
	std::string name;
	public:
	NinjaRule(std::string _name);
	void generate(std::string out, std::vector<std::string>& in,
	std::unordered_map<std::string, std::string> opts);
};

class Ninja: public Backend {
private:
	std::ofstream fs;

	friend NinjaRule;
public:
	Rule* create_rule(std::string name, std::unordered_map<std::string, std::string>& extra_props);
	Ninja(const char* file);
	~Ninja();
};
