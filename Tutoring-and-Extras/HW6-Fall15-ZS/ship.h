#ifndef  __SHIP_H__
#define __SHIP_H__

#include <string>
#include <iostream>

class Ship {
public:
	//Constructors
	Ship(const std::string& n);
	Ship(const std::string& n, int c, int r, const std::string& orient);

	//Accessors
	const std::string& getType() const { return type_; }
	const std::string& getOrientation() const { return orientation_; }
	int getLength() const { return length_; }
	int getRow() const { return row_; }
	int getColumn() const { return col_; }
	void print(std::ostream& ostr) const;
	char getChar(int index, const std::string& orient) const;

	//Mutators
	void setCoordinates(int c, int r);
	void setOrientation(const std::string& orient);
	void setType(const std::string& new_type);

	bool operator==(const Ship& s2) const;
	
private:
	std::string type_;
	int length_;
	int col_;
	int row_;
	std::string orientation_;

	int setLength(const std::string& shiptype);

};

bool by_greatest_length(const Ship& s1, const Ship& s2);

#endif
