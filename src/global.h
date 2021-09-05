#include <lua.hpp>
#include <stdbool.h>
#include <vector>
#include <variant>
#include <string>

extern int r;

void err(const char* string, ...);
void lerr(bool iserr, const char* string, ...);

// lua functions
void Lcheck_err(int result, lua_State* state);
std::vector<std::string> Ltable_to_vector(lua_State* L, int index);
void Lsetpath(lua_State* L, const char* path);

// generic lua type
class LGenericType {
private:
	enum Type {
		array,
		string,
		number
	};
	Type type;

	union {
		std::string str;
		int integer;
		std::vector<LGenericType> vec;
	} data;
public:
	LGenericType();
	~LGenericType();

	// conversion constructors

	void getfromidx(lua_State* L, int idx);
};

// vim: ft=cpp
