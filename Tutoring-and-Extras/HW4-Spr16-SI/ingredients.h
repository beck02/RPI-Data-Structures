#ifndef __ingredients_h
#define __ingredients_h

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
class Ingredients{
public:
	Ingredients(std::string aname, int aunits){
		units = aunits;
		name = aname;
	}
	int Getunits() const{
		return units;
	}
	std::string ingGetname() const{
		return name;
	}
	void Changeunits(int newunits){
		units += newunits;
	}
	void Subunits(int anewunits){
		units -= anewunits;
	}
	void newunit(int  aanewunits){
		units = aanewunits;
	}



private:
	int units;
	std::string name;
};

#endif