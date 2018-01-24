#ifndef  __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <iostream>
#include <string>
#include "ship.h"

class Board {
public:
	//Constructors
	Board(){}
	Board(int numcols, int numrows, const std::vector<int>& colmax, const std::vector<int>& rowmax, const std::vector<bool>& colunk, const std::vector<bool> rowunk);
	Board(const Board& old) : board_(old.board_), row_max_cap_(old.row_max_cap_), col_max_cap_(old.col_max_cap_)
				, row_curr_cap_(old.row_curr_cap_), col_curr_cap_(old.col_curr_cap_), constraints_(old.constraints_)
				, row_unknowns_(old.row_unknowns_), col_unknowns_(old.col_unknowns_) {}
	Board& operator=(const Board& old);

	//Accessors
	char getCellContents(int c, int r) const { return board_[c][r]; }
	int numColumns() const { return board_.size(); }
	int numRows() const { return board_[0].size(); }
	bool checkAdjacent(int c, int r) const;
	bool checkConstraint(int c, int r, char ch) const;
	void print(std::ostream& ostr) const;
	bool canPlace(int c, int r, const std::string& orientation, Ship& ship) const;
	bool areConstraintsSatisfied() const;
	bool areLinesFilled() const;

	//Mutators
	bool addShip(int c, int r, const std::string& orientation, Ship& ship);
	void placeShip(int c, int r, const std::string& orientation, Ship& ship);
	void removeShip(Ship& ship);
	void addConstraint(int c, int r, char ch);

	struct Constraint{
		Constraint(int c, int r, char cch) : col(c), row(r), ch(cch) {}
		int col, row;
		char ch;
	};

private:
	std::vector<std::vector<char> > board_;
	std::vector<int> row_max_cap_;
	std::vector<int> col_max_cap_;
	std::vector<int> row_curr_cap_;
	std::vector<int> col_curr_cap_;
	std::vector<Constraint> constraints_;
	std::vector<bool> row_unknowns_;
	std::vector<bool> col_unknowns_;
	
};

#endif
