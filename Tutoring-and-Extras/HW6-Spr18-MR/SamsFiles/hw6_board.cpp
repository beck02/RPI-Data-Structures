/*
@Author: Sam Saks-Fithian
Created on 3/20/18
Written as an example for RPI CS1200: Data Structures Homework 6 2018 tutoring
*/
#include <vector>
#include <iostream>
#include <string>
#include "hw6_ship.h"
#include "hw6_board.h"

//==========================================================================
//==========================================================================
//Static variable initializations

char Board::corner_char_ = '+';
char Board::top_char_ = '-';
char Board::side_char_ = '|';
char Board::empty_char_ = ' ';

//==========================================================================
//==========================================================================
//Constructors

//Board with trivial maxima and no unknowns, mainly for testing
Board::Board(int num_rows, int num_cols) {
	row_max_cap_ = std::vector<int>(num_rows, num_cols);
	col_max_cap_ = std::vector<int>(num_cols, num_rows);
	row_curr_cap_ = std::vector<int>(num_rows, 0);
	col_curr_cap_ = std::vector<int>(num_cols, 0);
	row_unknowns_ = std::vector<bool>(num_rows, false);
	col_unknowns_ = std::vector<bool>(num_cols, false);
	constraints_ = std::vector<Constraint>();
	board_ = std::vector<std::vector<char> >(num_rows, std::vector<char>(num_cols, empty_char_) );
}

//==========================================================================

Board::Board(int num_rows, int num_cols
	, const std::vector<int>& row_max, const std::vector<int>& col_max
	, const std::vector<bool>& row_unk, const std::vector<bool>& col_unk
	, const std::vector<Constraint>& constrs) 
			: row_max_cap_(row_max), col_max_cap_(col_max)
			, row_unknowns_(row_unk), col_unknowns_(col_unk) 
			, constraints_(constrs) {
	row_curr_cap_ = std::vector<int>(num_rows, 0);
	col_curr_cap_ = std::vector<int>(num_cols, 0);
	board_ = std::vector<std::vector<char> >(num_rows, std::vector<char>(num_cols, empty_char_) );
}

//==========================================================================

Board::Board(const Board& old) : board_(old.board_)
		, row_max_cap_(old.row_max_cap_), col_max_cap_(old.col_max_cap_)
		, row_curr_cap_(old.row_curr_cap_), col_curr_cap_(old.col_curr_cap_)
		, row_unknowns_(old.row_unknowns_), col_unknowns_(old.col_unknowns_)
		, constraints_(old.constraints_) {}

//==========================================================================

Board& Board::operator=(const Board& old){
	row_max_cap_ = old.row_max_cap_;
	col_max_cap_ = old.col_max_cap_;
	row_curr_cap_ = old.row_curr_cap_;
	col_curr_cap_ = old.col_curr_cap_;
	board_ = old.board_;
	row_unknowns_ = old.row_unknowns_;
	col_unknowns_ = old.col_unknowns_;
	constraints_ = old.constraints_;
	return *this;
}

//==========================================================================
//==========================================================================
//Mutators

bool Board::placeShip(int row_num, int col_num, const std::string& orientation, Ship& ship) {
	if(!canPlace(row_num, col_num, orientation, ship)){ return false; }
	int curr_row = row_num; int curr_col = col_num;
	int& dir_to_increment = (orientation == "vertical") ? curr_row : curr_col;
	for (int i = 0; i < ship.getLength(); ++i, ++dir_to_increment){
		board_[curr_row][curr_col] = getShipChar(ship, i, orientation);
		row_curr_cap_[curr_row]++;
		col_curr_cap_[curr_col]++;
	}
	ship.setCoordinates(row_num, col_num);
	ship.setOrientation(orientation);
	return true;
}

//==========================================================================

void Board::removeShip(Ship& ship) {
	if(!ship.isPlaced()){ return; }
	int row_num = ship.getRow(); int col_num = ship.getCol();
	int& dir_to_increment = (ship.getOrientation() == "vertical") ? row_num : col_num;
	for (int i = 0; i < ship.getLength(); ++i, ++dir_to_increment){
		board_[row_num][col_num] = empty_char_;
		row_curr_cap_[row_num]--;
		col_curr_cap_[col_num]--;
	}
	ship.setCoordinates(-1, -1);
	ship.setOrientation("");
}


//==========================================================================

void Board::addConstraint(int row_num, int col_num, char cch) {
	constraints_.push_back( Constraint(row_num, col_num, cch) );
}

//==========================================================================
//==========================================================================
//Valid spot helpers

bool Board::canPlace(int row_num, int col_num, const std::string& orientation, const Ship& ship) const {
	//not a valid orientation
	if ( !(orientation == "" && ship.getType() == "submarine") 
				&& (orientation != "vertical") 
			 	&& (orientation != "horizontal") ) { 
		return false; 
	}
	
	int curr_row = row_num; int curr_col = col_num;
	int& dir_to_increment = (orientation == "vertical") ? curr_row : curr_col;
	
	int row_len = 1; int col_len = 1;
	if(orientation == "vertical"){ col_len = ship.getLength(); } 
	else{ row_len = ship.getLength(); } 
	
	for (int i = 0; i < ship.getLength(); ++i, ++dir_to_increment){
		//any part of the ship would be out of bounds
		if(curr_row < 0 || curr_row >= numRows()){ return false; }
		if(curr_col < 0 || curr_col >= numCols()){ return false; }

		//any part of the ship would cross over another ship
		if(board_[curr_row][curr_col] != empty_char_){ return false; }

		//the row(s)/column(s) crossed by the ship don't have capacity for it
		if( (row_curr_cap_[curr_row] + row_len) > row_max_cap_[curr_row] ){ return false; }
		if( (col_curr_cap_[curr_col] + col_len) > col_max_cap_[curr_col] ){ return false; }
		
		//there is a constraint for this tile that doesn't match the part of the ship
		if( !checkForConstraint(curr_row, curr_col, getShipChar(ship, i, orientation)) ){ return false; }

		//any part of the ship would be directly adjacent to another ship
		if(!checkAdjacent(curr_row, curr_col)){ return false; }
	}
	return true;
}

//==========================================================================

bool Board::checkAdjacent(int row_num, int col_num) const {
	int curr_row, curr_col;
	for (int i = -1; i <= 1; ++i){
		for (int j = 0; j <= 1; ++j){
			curr_row = row_num + i;
			curr_col = col_num + j;
			if(curr_row < 0 || curr_row >= numRows()){ continue; }
			if(curr_col < 0 || curr_col >= numCols()){ continue; }
			if(board_[curr_row][curr_col] != empty_char_){ return false; }
		}
	}
	return true;
}

//==========================================================================

bool Board::checkForConstraint(int row_num, int col_num, char cch) const {
	for (int i = 0; i < constraints_.size(); ++i){
		const Constraint& curr_constraint = constraints_[i];
		if ( 	   (curr_constraint.row_ == row_num) 
				&& (curr_constraint.col_ == col_num) 
				&& (curr_constraint.chr_ != cch)      ){
			return false;
		}
	}
	return true;
}

//==========================================================================
//==========================================================================
//Completed board helpers

bool Board::areConstraintsSatisfied() const {
	for (int i = 0; i < constraints_.size(); ++i){
		const int& row_num = constraints_[i].row_;
		const int& col_num = constraints_[i].col_;
		char correct_char = (constraints_[i].chr_ == '_') ? empty_char_ : constraints_[i].chr_;
		if (board_[row_num][col_num] != correct_char){ return false; }
	}
	return true;
}

//==========================================================================

bool Board::areLinesFilled() const {
	for(int row_num = 0; row_num < numRows(); ++row_num){
		if(row_unknowns_[row_num]){ continue; }
		if(row_max_cap_[row_num] != row_curr_cap_[row_num]){ return false; }
	}
	for(int col_num = 0; col_num < numCols(); ++col_num){
		if(col_unknowns_[col_num]){ continue; }
		if(col_max_cap_[col_num] != col_curr_cap_[col_num]){ return false; }
	}
	return true;
}

//==========================================================================
//==========================================================================
//Additional helpers

void Board::print(std::ostream& ostr) const {
	std::string top_line = corner_char_ + std::string(numCols(), top_char_) + corner_char_;
	ostr << top_line << std::endl;
	for (int row_num = 0; row_num < numRows(); ++row_num){
		ostr << side_char_;
		for (int col_num = 0; col_num < numCols(); ++col_num){
			ostr << board_[row_num][col_num];
		}
		ostr << side_char_;
		if (row_unknowns_[row_num]){
			ostr << "?" << std::endl;
		} else{
			ostr << row_max_cap_[row_num] << std::endl;
		}
	}
	ostr << top_line << std::endl << " ";
	for (int col_num = 0; col_num < numCols(); ++col_num){
		if (col_unknowns_[col_num]){
			ostr << "?";
		} else{
			ostr << col_max_cap_[col_num];
		}
	}
	ostr << std::endl;
}

//==========================================================================
//==========================================================================





















