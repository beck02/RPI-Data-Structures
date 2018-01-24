#include <cassert>
#include "location.h"


// CONSTRUCTOR
Location::Location (int r, int c, int rot) : row(r), column(c), rotation(rot) { 
  assert (rotation == 0 || 
          rotation == 90 || 
          rotation == 180 || 
          rotation == 270); 
}

Location Location::getRelativeLocation(std::pair<int, int> offset){
	int r = row + offset.first;
	int c = column + offset.second;
	return Location(r, c, rotation);
}

std::pair<int, int> Location::offset(const Location &loc2) const{
	int r = row - loc2.row;
	int c = column - loc2.column;
	return std::make_pair(r, c);
}

bool operator==(const Location &loc1, const Location &loc2){
	return (loc1.row == loc2.row) && (loc1.column == loc2.column) && (loc1.rotation == loc2.rotation);
}

// helper function to output the placement of this tile within a solution
std::ostream& operator<<(std::ostream &ostr, const Location &loc) {
  ostr << "(" << loc.row << "," << loc.column << "," << loc.rotation << ")";
  return ostr;
}
