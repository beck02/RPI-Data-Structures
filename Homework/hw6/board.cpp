#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <fstream>

#include "board.h"


// this global variable is set in main.cpp and is adjustable from the command line
// (you are not allowed to make your own global variables)
extern int GLOBAL_TILE_SIZE;


// ==========================================================================
// CONSTRUCTOR
Board::Board(int i, int j) {
  board = std::vector<std::vector<Tile*> >((unsigned int)i,std::vector<Tile*>((unsigned int)j,NULL));
}


// ==========================================================================
// ACCESSORS
Tile* Board::getTile(int i, int j) const {
  assert (i >= 0 && i < numRows());
  assert (j >= 0 && j < numColumns());
  return board[i][j];
}

Tile* Board::getTile(Location l) const {
  int i = l.row; int j = l.column;
  assert (i >= 0 && i < numRows());
  assert (j >= 0 && j < numColumns());
  return board[i][j];
}

bool Board::inBounds(int i, int j) const{
  return (i >= 0) && (i < numRows()) && (j >= 0) && (j < numColumns());
}

bool Board::inBounds(Location l) const{
  int i = l.row; int j = l.column;
  return (i >= 0) && (i < numRows()) && (j >= 0) && (j < numColumns());
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

void Board::setTile(Location l, Tile* t) {
  int i = l.row; int j = l.column;
  assert (i >= 0 && i < numRows());
  assert (j >= 0 && j < numColumns());
  assert (t != NULL);
  assert (board[i][j] == NULL);
  board[i][j] = t;
}

void Board::removeTile(int i, int j) {
  assert (i >= 0 && i < numRows());
  assert (j >= 0 && j < numColumns());
  assert (board[i][j] != NULL);
  board[i][j] = NULL;
}

void Board::removeTile(Location l) {
  int i = l.row; int j = l.column;
  assert (i >= 0 && i < numRows());
  assert (j >= 0 && j < numColumns());
  assert (board[i][j] != NULL);
  board[i][j] = NULL;
}

// ==========================================================================
// PRINTING to cout
void Board::Print() const {
  for (int b = 0; b < numRows(); b++) {
    for (int i = 0; i < GLOBAL_TILE_SIZE; i++) {
      for (int j = 0; j < numColumns(); j++) {
        if (board[b][j] != NULL) {
          board[b][j]->printRow(std::cout,i);
        } else {
          if( j == 0 && ( (b == 0 && i == 0) || (b == numRows()-1 && i == GLOBAL_TILE_SIZE-1) ) ){
            std::cout << '+' << std::string(GLOBAL_TILE_SIZE-1, '-');
          } else if( j == numColumns()-1 && ( (b == 0 && i == 0) || (b == numRows()-1 && i == GLOBAL_TILE_SIZE-1) ) ){
            std::cout << std::string(GLOBAL_TILE_SIZE-1, '-') << '+';
          } else if( j == 0 ){
            std::cout << '|' << std::string(GLOBAL_TILE_SIZE-1, ' ');
          } else if( j == numColumns()-1 ){
            std::cout << std::string(GLOBAL_TILE_SIZE-1, ' ') << '|';
          } else if( (i == 0 && b == 0) || (i == GLOBAL_TILE_SIZE-1 && b == numRows()-1) ){
            std::cout << std::string(GLOBAL_TILE_SIZE, '-');
          } 
          else{
            std::cout << std::string(GLOBAL_TILE_SIZE, ' ');
          }
        }
      }
      std::cout << "\n";
    }
  }
  fflush(stdout);
}


//PRINTING to file
void Board::Print(std::ofstream& ostr) const {
  for (int b = 0; b < numRows(); b++) {
    for (int i = 0; i < GLOBAL_TILE_SIZE; i++) {
      for (int j = 0; j < numColumns(); j++) {
        if (board[b][j] != NULL) {
          board[b][j]->printRow(ostr,i);
        } else {
          ostr << std::string(GLOBAL_TILE_SIZE,' ');
        }
      }
      ostr << "\n";
    }
  }
}



// ==========================================================================
