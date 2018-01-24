#include <vector>
#include <iostream>
#include <string>
#include "ship.h"
#include "board.h"

//==========================================================================
//==========================================================================
//Constructor

Board::Board(int numcols, int numrows, const std::vector<int>& colmax, const std::vector<int>& rowmax, const std::vector<bool>& colunk, const std::vector<bool> rowunk){
	row_max_cap_ = rowmax;
	col_max_cap_ = colmax;
	row_unknowns_ = rowunk;
	col_unknowns_ = colunk;
	row_curr_cap_ = std::vector<int>(numrows, 0);
	col_curr_cap_ = std::vector<int>(numcols, 0);
	board_ = std::vector<std::vector<char> >(numcols, std::vector<char>(numrows, ' ') );
}

//==========================================================================

Board& Board::operator=(const Board& old){
	row_max_cap_ = old.row_max_cap_;
	col_max_cap_ = old.col_max_cap_;
	row_curr_cap_ = old.row_curr_cap_;
	col_curr_cap_ = old.col_curr_cap_;
	board_ = old.board_;
	row_unknowns_ = old.row_unknowns_;
	col_unknowns_ = old.col_unknowns_;
	return *this;
}

//==========================================================================
//==========================================================================
//Accessors

void Board::print(std::ostream& ostr) const{
	ostr << "+" << std::string(board_.size(), '-') << "+" << std::endl;
	for(int r = 0; r < board_[0].size(); r++){
		ostr << "|";
		for(int c = 0; c < board_.size(); c++){
			ostr << board_[c][r];
		}
		ostr << "|" << row_max_cap_[r];
		// ostr << " " << row_curr_cap_[r];
		ostr << std::endl;
	}
	ostr << "+" << std::string(board_.size(), '-') << "+" << std::endl << " ";
	for(int c = 0; c < board_.size(); c++){
		ostr << col_max_cap_[c];
	}
	ostr << std::endl;
	/*ostr << " ";
	for(int c = 0; c < board_.size(); c++){
		ostr << col_curr_cap_[c];
	}
	ostr << std::endl;*/
}

//==========================================================================

bool Board::checkAdjacent(int c, int r) const{
	for(int i = -1; i <= 1; i++){
		for(int j = -1; j <= 1; j++){
			if(c+i >= board_.size() || c+i < 0){ continue; }
			if(r+j >= board_[c+i].size() || r+j < 0){ continue; }
			if(board_[c+i][r+j] != ' '){ return false; }
		}
	}
	return true;
}

//==========================================================================

bool Board::checkConstraint(int c, int r, char ch) const{
	// std::cout << "  (" << c << ", " << r << ", " << ch << ") --";
	for(int i = 0; i < constraints_.size(); ++i){
		// std::cout << "  (" << constraints_[i].col << ", " << constraints_[i].row << ", " << constraints_[i].ch << ") ";
		if( (constraints_[i].col == c) && (constraints_[i].row == r) ){
			// std::cout << " spot match ";
			if(constraints_[i].ch == ch){ 
				/*std::cout << " char match" << std::endl;*/ 
				return true; 
			} else{ 
				/*std::cout << " char fail" << std::endl;*/ 
				return false; 
			}
		}
		// std::cout << " spot fail " << std::endl;
	}
	return true;
}

//==========================================================================

bool Board::areConstraintsSatisfied() const{
	for(int i = 0; i < constraints_.size(); ++i){
		char c = constraints_[i].col;
		char r = constraints_[i].row;
		if(board_[c][r] != constraints_[i].ch){ return false; }
	}
	return true;
}

//==========================================================================

bool Board::areLinesFilled() const{
	for(int r = 0; r < row_max_cap_.size(); ++r){
		if(row_unknowns_[r]){ continue; }
		if(row_max_cap_[r] != row_curr_cap_[r]){ return false; }
	}
	for(int c = 0; c < col_max_cap_.size(); ++c){
		if(col_unknowns_[c]){ continue; }
		if(col_max_cap_[c] != col_curr_cap_[c]){ return false; }
	}
	return true;
}

//==========================================================================

bool Board::canPlace(int c, int r, const std::string& orientation, Ship& ship) const{
	int len = ship.getLength();
	for(int i = 0; i < len; i++){
		char ch = ship.getChar(i, orientation);
		int clen = 1; int rlen = 1;
		int currCol = c; int currRow = r;
		if(orientation == "horizontal"){ currCol = c + i; rlen = len; }
		if(orientation == "vertical"){ currRow = r + i; clen = len; }

		if(currCol >= board_.size() || currCol < 0){ return false; }
		if(currRow >= board_[currCol].size() || currRow < 0){ return false; }
		if(board_[currCol][currRow] != ' '){ return false; }
		if(row_curr_cap_[currRow] + rlen > row_max_cap_[currRow]){ return false; }
		if(col_curr_cap_[currCol] + clen > col_max_cap_[currCol]){ return false; }
		if( checkConstraint(currCol, currRow, ch ) == false ){ return false; }
		if( checkAdjacent(currCol, currRow) == false ){ return false; }
	}
	return true;
}

//==========================================================================
//==========================================================================
//Mutators

bool Board::addShip(int c, int r, const std::string& orientation, Ship& ship){
	// ship.setCoordinates(c, r);
	// ship.print(std::cout);
	// ship.setCoordinates(-1,-1);

	if(ship.getType() == "submarine"){
		if(c >= board_.size() || c < 0){ return false; }
		if(r >= board_[c].size() || r < 0){ return false; }
		if(board_[c][r] != ' '){ return false; }
		if(row_curr_cap_[r]+1 > row_max_cap_[r]){ return false; }
		if(col_curr_cap_[c]+1 > col_max_cap_[c]){ return false; }
		if( !checkConstraint(c, r, 'o' ) ){ return false; }
		if( !checkAdjacent(c, r) ){ return false; }
		board_[c][r] = 'o';
		col_curr_cap_[c]++;
		row_curr_cap_[r]++;
		ship.setCoordinates(c, r);
		ship.setOrientation(orientation);
		// print(std::cout);
		return true;
	}
	for(int i = 0; i < ship.getLength(); i++){
		char ch = ship.getChar(i, orientation);
		if(orientation == "horizontal"){
			if(c+i >= board_.size() || c+i < 0){ return false; }
			if(r >= board_[c+i].size() || r < 0){ return false; }
			if(board_[c+i][r] != ' '){ return false; }
			if(ship.getLength() + row_curr_cap_[r] > row_max_cap_[r]){ return false; }
			if(col_curr_cap_[c+i]+1 > col_max_cap_[c+i]){ return false; }
			if( !checkConstraint(c+i, r, ch) ){ return false; }
			if( !checkAdjacent(c+i, r) ){ return false; }
		} else if(orientation == "vertical"){
			if(c >= board_.size() || c < 0){ return false; }
			if(r+i >= board_[c].size() || r+i < 0){ return false; }
			if(board_[c][r+i] != ' '){ return false; }
			if(ship.getLength() + col_curr_cap_[c] > col_max_cap_[c]){ return false; }
			if(row_curr_cap_[r+i]+1 > row_max_cap_[r+i]){ return false; }
			if( !checkConstraint(c, r+i, ch ) ){ return false; }
			if( !checkAdjacent(c, r+i) ){ return false; }
		}
	}
	//actually add chars to the boards
	//start and end pieces
	if(orientation == "horizontal"){
		board_[c][r] = '<';
		board_[c + ship.getLength()-1][r] = '>';
		row_curr_cap_[r] += 2;
		col_curr_cap_[c]++;
		col_curr_cap_[c+ship.getLength()-1]++;
	} else if(orientation == "vertical"){
		board_[c][r] = '^';
		board_[c][r + ship.getLength()-1] = 'v';
		col_curr_cap_[c] += 2;
		row_curr_cap_[r]++;
		row_curr_cap_[r+ship.getLength()-1]++;
	}
	//middle pieces
	for(int i = 1; i < ship.getLength()-1; i++){
		int currCol = c, currRow = r;
		if(orientation == "horizontal"){ currCol = c + i; }
		else if(orientation == "vertical"){ currRow = r + i; }
		board_[currCol][currRow] = 'X';
		col_curr_cap_[currCol]++;
		row_curr_cap_[currRow]++;
	}

	ship.setCoordinates(c, r);
	ship.setOrientation(orientation);
	// ship.print(std::cout);
	// print(std::cout);
	return true;
}

//==========================================================================

void Board::placeShip(int c, int r, const std::string& orientation, Ship& ship){
	for(int i = 0; i < ship.getLength(); i++){
		int currCol = c; int currRow = r;
		if(orientation == "horizontal"){ currCol = c + i; }
		if(orientation == "vertical"){ currRow = r + i; }

		board_[currCol][currRow] = ship.getChar(i, orientation);
		col_curr_cap_[currCol]++;
		row_curr_cap_[currRow]++;
	}
	ship.setCoordinates(c, r);
	ship.setOrientation(orientation);
}

//==========================================================================

void Board::removeShip(Ship& ship){
	for(int i = 0; i < ship.getLength(); i++){
		int currCol = ship.getColumn();
		int currRow = ship.getRow();
		if(ship.getOrientation() == "horizontal"){ currCol = currCol + i; }
		if(ship.getOrientation() == "vertical"){ currRow = currRow + i; }
		board_[currCol][currRow] = ' ';
		row_curr_cap_[currRow]--;
		col_curr_cap_[currCol]--;
	}

	ship.setCoordinates(-1, -1);
	ship.setOrientation("");
	// std::cout << "Removing " << ship.getType() << std::endl;
}

//==========================================================================

void Board::addConstraint(int c, int r, char ch){
	Constraint new_constraint(c, r, ch);
	constraints_.push_back(new_constraint);
}

//==========================================================================
//==========================================================================









