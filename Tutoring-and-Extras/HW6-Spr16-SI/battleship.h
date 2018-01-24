#ifndef __ingredients_h
#define __ingredients_h

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

//created an Battleship class to keep track of the stats of each battlesip
class Battleship{
public:
	//constructor
	Battleship(std::string aname, int asize){
		size = asize;
		name = aname;
		orientation = "horizontal";
	}
	//returns units
	int get_size() const{
		return size;
	}
	//returns ingredient name
	std::string get_name() const{
		return name;
	}
	//adds unit amounts
	std::string get_orientation() const{
		return orientation;
	}
	void change_orientation(){
		if(orientation == "horizontal"){
			orientation = "vertical";
		}
		else{
			orientation = "horizontal";
		}
	}
	std::string get_ship_string();


private:
	int size;
	std::string name;
	std::string orientation;
};

#endif
