#include "ninja.h"
#include <lua.hpp>

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>

extern Backend* backend;

//
// NinjaRule functions
//
NinjaRule::NinjaRule(std::string _name) {
	this->name = _name;
}

void NinjaRule::generate(std::string out, std::vector<std::string>& in) {
	std::cerr << "-- Generating build statement for " << out << "\n";
	auto& fs = ((Ninja*)backend)->fs;

	fs << "build " << out << ": " << this->name;
	for(auto& i: in) {
		fs << " " << i ;
	}
	fs << "\n";
}

//
// Ninja functions
//

Ninja::Ninja(const char* file) : fs(file){
	this->fs
	<< "#\n"
	<< "# Generated by tachyon\n"
	<< "# DO NOT EDIT MANUALLY\n"
	<< "#\n";
}

Rule* Ninja::create_rule(std::string name, std::map<std::string, std::string>& extra_props) {
	std::cerr << "-- Generating rule for " << name << std::endl;

	this->fs << "rule " << name << "\n";
	for (auto& kv: extra_props) {
		this->fs << " " << kv.first << " = " << kv.second << "\n";
	}
	fs << "\n";

	NinjaRule* ret = new NinjaRule{name};

	return (Rule*)ret;
}
