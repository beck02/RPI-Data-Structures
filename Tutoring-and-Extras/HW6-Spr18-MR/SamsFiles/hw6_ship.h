/*
@Author: Sam Saks-Fithian
Created on 3/20/18
Written as an example for RPI CS1200: Data Structures Homework 6 2018 tutoring
*/
#ifndef __SHIP_H__
#define __SHIP_H__

#include <string>
#include <iostream>

class Ship {
public:
	//Constructors
	Ship();
	Ship(const std::string& ship_type);
	Ship(const std::string& ship_type, int row_num, int col_num, const std::string& orient);
	Ship& operator=(const Ship& old);

	//Accessors
	const std::string& getType() const { return type_; }
	const std::string& getOrientation() const { return orientation_; }
	int getID() const { return id_; }
	int getLength() const { return length_; }
	int getRow() const { return row_; }
	int getCol() const { return col_; }

	//Mutators
	void setCoordinates(int row_num, int col_num) { row_ = row_num; col_ = col_num; }
	void setOrientation(const std::string& orient) { orientation_ = orient; }
	void setType(const std::string& new_type) { type_ = new_type; setLength(); }
	int setLength();
	
	//Operators
	//Ships are counted as equivalent based on type and position, regardless of actual ID
	bool operator==(const Ship& s2) const;
	bool operator!=(const Ship& s2) const;

	//Other Helpers
	void printData(std::ostream& ostr, bool print_id = false) const;
	bool isPlaced() const { return row_ >= 0 && col_ >= 0; };

	const static std::string SHIP_TYPES[];
	
private:
	const int id_;
	std::string orientation_;
	std::string type_;
	int length_;
	int row_;
	int col_;

	static int static_id_count;

};

char getShipChar(const Ship & ship, int index_in_ship, const std::string& orient);
bool by_greatest_length(const Ship& s1, const Ship& s2);

#endif
