/*
@Author: Sam Saks-Fithian
Created on 3/20/18
Written as an example for RPI CS1200: Data Structures Homework 6 2018 tutoring
*/
#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <iostream>
#include <string>
#include "hw6_ship.h"

//==========================================================================

struct Constraint{
	Constraint(int row_num, int col_num, char cch) : row_(row_num), col_(col_num), chr_(cch) {}
	Constraint(const Constraint& old) : row_(old.row_), col_(old.col_), chr_(old.chr_) {}
	int row_, col_;
	char chr_;
};

//==========================================================================

class Board {
public:
	//Constructors
	Board(int num_rows, int num_cols);
	Board(int num_rows, int num_cols
		, const std::vector<int>& row_max, const std::vector<int>& col_max
		, const std::vector<bool>& row_unk, const std::vector<bool>& col_unk
		, const std::vector<Constraint>& constrs = std::vector<Constraint>());
	Board(const Board& old);
	Board& operator=(const Board& old);

	//Accessors
	char getCellContents(int row_num, int col_num) const { return board_[row_num][col_num]; }
	int numRows() const { return board_.size(); }
	int numCols() const { return board_[0].size(); }

	//Mutators
	bool placeShip(int row_num, int col_num, const std::string& orientation, Ship& ship);
	void removeShip(Ship& ship);
	void addConstraint(int row_num, int col_num, char cch);

	//Helpers for checking if a location is a valid spot to place a ship
	bool canPlace(int row_num, int col_num, const std::string& orientation, const Ship& ship) const;
	bool checkAdjacent(int row_num, int col_num) const;
	bool checkForConstraint(int row_num, int col_num, char cch) const;

	//Helpers for checking if the board is completed
	bool areConstraintsSatisfied() const;
	bool areLinesFilled() const;

	//Additional Helpers
	void print(std::ostream& ostr) const;

	static char corner_char_;
	static char top_char_;
	static char side_char_;
	static char empty_char_;

private:
	std::vector<std::vector<char> > board_;
	std::vector<int> row_max_cap_;
	std::vector<int> col_max_cap_;
	std::vector<int> row_curr_cap_;
	std::vector<int> col_curr_cap_;
	std::vector<bool> row_unknowns_;
	std::vector<bool> col_unknowns_;
	std::vector<Constraint> constraints_;

};


#endif





