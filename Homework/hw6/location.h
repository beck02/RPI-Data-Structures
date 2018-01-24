#ifndef _LOCATION_H_
#define _LOCATION_H_

#include <iostream>
#include <utility>


// Tiny all-public class to store the grid coordinates and rotation
// for placing a tile onto the board
class Location {
public:
  Location (int r, int c, int rot);
  std::pair<int, int> offset(const Location &loc2) const;
  Location getRelativeLocation(std::pair<int, int> offset);
  int row;
  int column;
  int rotation; // should be 0, 90, 180, or 270
};


// Check of these two locations are the same
bool operator==(const Location &loc1, const Location &loc2);

// Helper function to output a location as part of a puzzle solution
std::ostream& operator<<(std::ostream &ostr, const Location &loc);


#endif
