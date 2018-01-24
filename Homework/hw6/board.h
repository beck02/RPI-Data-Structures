#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include "tile.h"
#include "location.h"


// This class stores a grid of Tile pointers, which are NULL if the
// grid location does not (yet) contain a tile

class Board {
public:

  // CONSTRUCTOR
  // takes in the dimensions (height & width) of the board
  Board(int i, int j);

  // ACCESSORS
  int numRows() const { return board.size(); }
  int numColumns() const { return board[0].size(); }
  Tile* getTile(int i, int j) const;
  Tile* getTile(Location l) const;
  bool inBounds(int i, int j) const;
  bool inBounds(Location l) const;

  // MODIFIERS
  void setTile(int i, int j, Tile* t);
  void setTile(Location l, Tile* t);
  void removeTile(int i, int j);
  void removeTile(Location l);

  // FOR PRINTING
  void Print() const;
  void Print(std::ofstream& ostr) const;

private:

  // REPRESENTATION
  std::vector<std::vector<Tile*> > board;
};


#endif
