#include <cassert>
#include <cstdlib>
#include <cstdio>

#include "board.h"


// this global variable is set in main.cpp and is adjustable from the command line
// (you are not allowed to make your own global variables)
extern int GLOBAL_TILE_SIZE;


// ==========================================================================
// CONSTRUCTOR
Board::Board(int i, int j) {
  board = std::vector<std::vector<Tile*> >
    ( (unsigned int)i,
      std::vector<Tile*>((unsigned int)j,NULL) );
}


// ==========================================================================
// ACCESSORS
Tile* Board::getTile(int i, int j) const {
  assert (i >= 0 && i < numRows());
  assert (j >= 0 && j < numColumns());
  return board[i][j];
}


// ==========================================================================
// MODIFIERS
void Board::setTile(int i, int j, Tile* t) {
  assert (i >= 0 && i < numRows());
  assert (j >= 0 && j < numColumns());
  assert (t != NULL);
  assert (board[i][j] == NULL);
  board[i][j] = t;
}

void Board::removeTile(int i, int j) {
  assert (i >= 0 && i < numRows());
  assert (j >= 0 && j < numColumns());
  board[i][j] = NULL;
}

// ==========================================================================
// PRINTING
void Board::Print() const {
  for (int b = 0; b < numRows(); b++) {
    for (int i = 0; i < GLOBAL_TILE_SIZE; i++) {
      for (int j = 0; j < numColumns(); j++) {
        if (board[b][j] != NULL) {
          board[b][j]->printRow(std::cout,i);
        } else {
          if(i == 0 || i == GLOBAL_TILE_SIZE-1){
            std::cout << '+' << std::string(GLOBAL_TILE_SIZE-2, '-') << '+';
          } else if(i == GLOBAL_TILE_SIZE/2){
            std::cout << '|' << std::string(GLOBAL_TILE_SIZE/2-3, ' ') << '(' << b << ',' << j << ')' << std::string(GLOBAL_TILE_SIZE/2-3, ' ') << '|';
          } else{
            std::cout << '|' << std::string(GLOBAL_TILE_SIZE-2, ' ') << '|';
          }
        }
      }
      std::cout << "\n";
    }
  }
  fflush(stdout);
}

// ==========================================================================
