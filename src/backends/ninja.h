#include <fstream>

#include "backends/backend.h"

class Ninja : public Backend{
private:
	std::ofstream fs;
public:
	Ninja(const char* file);
};
