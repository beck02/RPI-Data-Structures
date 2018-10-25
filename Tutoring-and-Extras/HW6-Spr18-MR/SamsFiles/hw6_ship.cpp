/*
@Author: Sam Saks-Fithian
Created on 3/20/18
Written as an example for RPI CS1200: Data Structures Homework 6 2018 tutoring
*/
#include <string>
#include <iostream>
#include "hw6_ship.h"

//==========================================================================
//==========================================================================
//Static variable initializations

int Ship::static_id_count = 1;
const std::string Ship::SHIP_TYPES[] = {"submarine", "destroyer", "cruiser"
								, "battleship", "carrier", "cargo", "tanker"};

//==========================================================================
//==========================================================================
//Constructors

Ship::Ship() : id_(static_id_count++), orientation_(""), type_("unknown"), row_(-1), col_(-1) {
	setLength();
}

//==========================================================================

Ship::Ship(const std::string& ship_type) : id_(static_id_count++) 
				, orientation_(""), type_(ship_type), row_(-1), col_(-1) {
	setLength();
}

//==========================================================================

Ship::Ship(const std::string& ship_type, int row_num, int col_num, const std::string& orient)
				: id_(static_id_count++), orientation_(orient)
				, type_(ship_type), row_(row_num), col_(col_num) {
	setLength();
}

//==========================================================================
//==========================================================================
//Operators

Ship& Ship::operator=(const Ship& old){
	orientation_ = old.orientation_;
	type_ = old.type_;
	length_ = old.length_;
	row_ = old.row_;
	col_ = old.col_;
	return *this;
}

bool Ship::operator==(const Ship& s2) const { 
	return (type_ == s2.type_) && (length_ == s2.length_) 
			&& (row_ == s2.row_) && (col_ == s2.col_) 
			&& (orientation_ == s2.orientation_); 
}

bool Ship::operator!=(const Ship& s2) const { 
	return (type_ != s2.type_) || (length_ != s2.length_) 
			|| (row_ != s2.row_) || (col_ != s2.col_) 
			|| (orientation_ != s2.orientation_); 
}


//==========================================================================
//==========================================================================
//Mutators

int Ship::setLength(){
	length_ = -1;
	if(type_ == "submarine"){ length_ = 1; }
	else if(type_ == "destroyer"){ length_ = 2; }
	else if(type_ == "cruiser"){ length_ = 3; }
	else if(type_ == "battleship"){ length_ = 4; }
	else if(type_ == "carrier"){ length_ = 5; }
	else if(type_ == "cargo"){ length_ = 6; }
	else if(type_ == "tanker"){ length_ = 7; }
	return length_;
}

//==========================================================================
//==========================================================================
//Helpers

void Ship::printData(std::ostream& ostr, bool print_id) const {
	std::string id_text = "";
	if(print_id){ id_text = "id" + std::to_string(id_) + ": "; }
	std::string padding(11-type_.size(), ' ');
	ostr << id_text << type_ << padding << row_ << " " 
		 << col_ << " " << orientation_ << std::endl;
}

//==========================================================================
//==========================================================================
//Non-class Helpers

char getShipChar(const Ship & ship, int index_in_ship, const std::string& orient){
	if (index_in_ship >= ship.getLength()){ return '!'; } //invalid request
	if(ship.getType() == "submarine"){ return 'o'; }
	if(index_in_ship == 0){
		if(orient == "horizontal"){ return '<'; }
		if(orient == "vertical"){ return '^'; }
	} else if(index_in_ship == ship.getLength()-1){
		if(orient == "horizontal"){ return '>'; }
		if(orient == "vertical"){ return 'v'; }
	}
	return 'X';
}

//==========================================================================

bool by_greatest_length(const Ship& s1, const Ship& s2){ 
	return s1.getLength() > s2.getLength();
}

//==========================================================================
//==========================================================================


