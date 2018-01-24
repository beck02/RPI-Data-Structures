#include <string>
#include <iostream>
#include "ship.h"

//==========================================================================
//==========================================================================
//Constructors

Ship::Ship(const std::string& n){
	type_ = n;
	setLength(type_);
	col_ = -1;
	row_ = -1;
	orientation_ = "";
}

//==========================================================================

Ship::Ship(const std::string& n, int c, int r, const std::string& orient){
	type_ = n;
	setLength(type_);
	col_ = c;
	row_ = r;
	orientation_ = orient;
}

//==========================================================================
//==========================================================================
//Accessors

char Ship::getChar(int index, const std::string& orient) const {
	if(type_ == "submarine"){ return 'o'; }
	if(index == 0){
		if(orient == "horizontal"){ return '<'; }
		if(orient == "vertical"){ return '^'; }
	} else if(index == length_-1){
		if(orient == "horizontal"){ return '>'; }
		if(orient == "vertical"){ return 'v'; }
	} else{
		return 'X';
	}
	return 'X';
}

//==========================================================================
//==========================================================================
//Helpers

int Ship::setLength(const std::string& shiptype){
	length_ = -1;
	if(shiptype == "submarine"){ length_ = 1; }
	else if(shiptype == "destroyer"){ length_ = 2; }
	else if(shiptype == "cruiser"){ length_ = 3; }
	else if(shiptype == "battleship"){ length_ = 4; }
	else if(shiptype == "carrier"){ length_ = 5; }
	else if(shiptype == "cargo"){ length_ = 6; }
	else if(shiptype == "tanker"){ length_ = 7; }
	return length_;
}

//==========================================================================

void Ship::print(std::ostream& ostr) const{
	std::string padding(11-type_.size(), ' ');
	ostr << type_ << padding << row_ << " " << col_ << " " << orientation_ << std::endl;
}

//==========================================================================

bool by_greatest_length(const Ship& s1, const Ship& s2){ 
	return s1.getLength() > s2.getLength();
}

//==========================================================================
//==========================================================================









