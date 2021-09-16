#define CATCH_CONFIG_RUNNER 
#include <catch2/catch.hpp>

#include <iostream>

int main(int argc, char *argv[]) {
	Catch::Session session;
	int result = session.applyCommandLine(argc, argv);

	if(result != 0) {
		return result;
	}

	int numFailed = session.run();

	return numFailed;
	return 0;
}
