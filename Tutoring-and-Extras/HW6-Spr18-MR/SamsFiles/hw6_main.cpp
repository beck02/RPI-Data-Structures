/*
@Author: Sam Saks-Fithian
Created on 3/20/18
Written as an example for RPI CS1200: Data Structures Homework 6 2018 tutoring
*/
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "hw6_board.h"
#include "hw6_ship.h"

//==========================================================================
//==========================================================================
//Helper struct

struct Solution{
	Solution(const Board& solved_board, const std::vector<Ship>& placed_ships)
		: board_(solved_board), ships_(placed_ships) {}
	Board board_;
	std::vector<Ship> ships_;
};

//==========================================================================
//==========================================================================
//Function prototypes

bool uniqueSolution(const Solution& new_sol, const std::vector<Solution>& valid_solutions);
void printSolution(std::ostream& ostr, const Solution& sol);

void readInput(std::ifstream& infile, Board& board, std::vector<Ship>& all_ships);

bool doneLookingForSolutions(bool find_all, const std::vector<Solution>& solutions);
void recursivePlaceShip(bool find_all, Board& board, std::vector<Ship>& all_ships
				, int curr_ship_num, std::vector<Solution>& valid_solutions );

//==========================================================================
//==========================================================================

int main(int argc, char* argv[]) {
	if(argc != 2 && argc != 3){
		std::cerr << "Usage: " << argv[0] << " infile [-find_all_solutions]\n" 
				  << "    infile:   name of input file\n" 
				  << "    -find_all_solutions:   optional flag to indicate whether\n"
				  << "       the program should output one solution, or all unique\n"
				  << "       solutions. Can use any flag text that includes 'all'." << std::endl;
		exit(1);
	}

	std::ifstream infile;
	infile.open(argv[1]);
	if (!infile.good()){
		std::cerr << "Can't open" << argv[1] << "to read.\n";
		exit(1);
	}

	bool find_all = false;
	if(argc == 3){ 
		std::string fas = argv[2];
		find_all = (fas.find("all") != std::string::npos); 
	}

	Board board(1, 1);
	std::vector<Ship> ships;
	readInput(infile, board, ships);

	// board.print(std::cout);
	// for (int i = 0; i < ships.size(); ++i){
	// 	ships[i].printData(std::cout);
	// }

	std::sort(ships.begin(), ships.end(), by_greatest_length);

	std::vector<Solution> solutions;
	recursivePlaceShip(find_all, board, ships, 0, solutions);

	if (solutions.empty()){ // no solutions found
		std::cout << "No solutions." << std::endl;
	} else if ( !find_all ){ // only need one solution
		printSolution(std::cout, solutions[0]);
	} else { // printing all solutions
		for (int i = 0; i < solutions.size(); ++i){
			printSolution(std::cout, solutions[i]);
		}
		std::cout << "Found " << solutions.size() << " solution(s)" << std::endl;
	}

	return 0;
}

//==========================================================================
//==========================================================================

bool uniqueSolution(const Solution& new_sol, const std::vector<Solution>& valid_solutions){
	// go through all previously found solutions
	for (int curr_sol_num = 0; curr_sol_num < valid_solutions.size(); ++curr_sol_num){
		
		int num_matched_ships = 0;
		const std::vector<Ship>& curr_sol_ships = valid_solutions[curr_sol_num].ships_;
		// go through all ships in the new solution
		for(int nn = 0; nn < new_sol.ships_.size(); ++nn){
			// go through all ships in the current old solution
			for(int cn = 0; cn < curr_sol_ships.size(); ++cn){
				// check if ships match
				if(new_sol.ships_[nn] == curr_sol_ships[cn]){
					++num_matched_ships;
					break;
				}
			} // end for all old ships
		} // end for all new ships
		
		// looked at all ship pairs, so see if all ships had a match
		if(num_matched_ships >= new_sol.ships_.size()){
			// all ships matched, so duplicate solution
			return false;
		}
	} // end for all old solutions
	return true;
} 

//==========================================================================

void printSolution(std::ostream& ostr, const Solution& sol){
	ostr << "Solution:" << std::endl;
	for (int i = 0; i < sol.ships_.size(); ++i){
		sol.ships_[i].printData(ostr);
	}
	sol.board_.print(ostr);
	ostr << std::endl;
}

//==========================================================================

void readInput(std::ifstream& infile, Board & board, std::vector<Ship>& all_ships){
	std::string token;
	infile >> token; // token == "board"
	int num_rows, num_cols;
	infile >> num_rows;
	infile >> num_cols;

	//TODO: add check for unknowns

	infile >> token; // token == "rows"
	std::vector<int> rows_max;
	std::vector<bool> rows_unk(num_rows, false);
	for (int i = 0; i < num_rows; ++i){
		int curr_row_max;
		infile >> curr_row_max;
		rows_max.push_back(curr_row_max);
	}

	infile >> token; // token == "cols"
	std::vector<int> cols_max;
	std::vector<bool> cols_unk(num_cols, false);
	for (int i = 0; i < num_cols; ++i){
		int curr_col_max;
		infile >> curr_col_max;
		cols_max.push_back(curr_col_max);
	}
	board = Board(num_rows, num_cols, rows_max, cols_max, rows_unk, cols_unk);
	
	while( (infile >> token) && (token != "constraint") ){
		all_ships.push_back( Ship(token) );
	}

	// token == eof or token == "constraint"
	while( infile >> token ){
		//token == "constraint"
		int row, col;
		infile >> row;
		infile >> col;
		char cch;
		infile >> cch;
		board.addConstraint(row, col, cch);
		infile >> token; // token == "constraint" on next line or eof
	}
}

//==========================================================================

bool doneLookingForSolutions(bool find_all, const std::vector<Solution>& solutions){
	bool have_sol = (solutions.size() >= 1);
	// std::cout << "have_sol: " << have_sol << ", find_all: " << find_all << std::endl;
	return have_sol && !find_all;
}

//==========================================================================

void recursivePlaceShip(bool find_all, Board& board, std::vector<Ship>& all_ships
				, int curr_ship_num, std::vector<Solution>& valid_solutions ){
	// if we only need one solution and we've already found one, stop
	if( doneLookingForSolutions(find_all, valid_solutions) ){ return; }

	// base case: all ships have been placed
	if( curr_ship_num >= all_ships.size() ){
		Solution new_sol(board, all_ships);
		// check to make sure solution is valid and unique
		if( board.areLinesFilled() && board.areConstraintsSatisfied() && uniqueSolution(new_sol, valid_solutions) ){
			valid_solutions.push_back( new_sol );
		}
		return;
	} // end base case
	
	// main recursive section: each iteration tries to place a single ship on
	// the board in each possible grid square and in each orientation
	Ship& curr_ship = all_ships[curr_ship_num];
	for(int row_num = 0; row_num < board.numRows(); ++row_num){
		for(int col_num = 0; col_num < board.numCols(); ++col_num){
			//TODO: add things to deal with unknowns

			// if the ship is a submarine, don't check orientations
			if (curr_ship.getType() == "submarine"){
				if( board.canPlace(row_num, col_num, "", curr_ship) ){
					board.placeShip(row_num, col_num, "", curr_ship);
					recursivePlaceShip(find_all, board, all_ships, curr_ship_num + 1, valid_solutions);
					board.removeShip(curr_ship);
					if( doneLookingForSolutions(find_all, valid_solutions) ){ return; }
				}
			} // end if sub 
			else{
				if( board.canPlace(row_num, col_num, "horizontal", curr_ship) ){
					board.placeShip(row_num, col_num, "horizontal", curr_ship);
					recursivePlaceShip(find_all, board, all_ships, curr_ship_num + 1, valid_solutions);
					board.removeShip(curr_ship);
					if( doneLookingForSolutions(find_all, valid_solutions) ){ return; }
				} // end if horizontal
				if( board.canPlace(row_num, col_num, "vertical", curr_ship) ){
					board.placeShip(row_num, col_num, "vertical", curr_ship);
					recursivePlaceShip(find_all, board, all_ships, curr_ship_num + 1, valid_solutions);
					board.removeShip(curr_ship);
					if( doneLookingForSolutions(find_all, valid_solutions) ){ return; }
				} // end if vertical
			} // end else (not sub)
		} // end for cols
	} // end for rows
} // end recursion

//==========================================================================
//==========================================================================
















