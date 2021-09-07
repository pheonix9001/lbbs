#include <lua.hpp>
#include <vector>
#include <variant>
#include <string>

extern int r;

void err(const char* string, ...);
void lerr(bool iserr, const char* string, ...);

// lua functions
void Lcheck_err(int result, lua_State* state);
void Lsetpath(lua_State* L, const char* path);
std::vector<std::string> Ltable_to_vector(lua_State* L, int index);

// generic lua type
class LGenericType {
	private:
	public:
	std::variant<int, std::string, std::vector<LGenericType>> data;
	LGenericType(std::string str) { this->data = str; }

	void getfromidx(lua_State* L, int idx);
};

// vim: ft=cpp
