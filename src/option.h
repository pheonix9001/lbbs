#include <variant>
#include <string>
#include <lua.hpp>

// generic option type
class OptionVal {
	private:
	public:
	std::variant<int, std::string> data;
	OptionVal(std::string str) { this->data = str; }

	void getfromidx(lua_State* L, int idx);
};
