#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include "board.h"
#include "ship.h"

//==========================================================================

struct Solution{
	Solution(const Board& b, const std::vector<Ship>& s) : board(b), ships(s) {}
	Board board;
	std::vector<Ship> ships;
};

//==========================================================================

//function prototypes
void readInput(std::ifstream& infile, Board& board, std::vector<Ship>& ships);
void printSolution(std::ostream& ostr, const Solution& sol);
bool validSolution(const Solution& sol);
bool newSolution(const Solution& sol, const std::vector<Solution>& solutions);
void placeHelper(Board& board, std::vector<Ship>& ships, int shipNum, std::vector<Solution>& solutions, bool find_all_solutions, int c, int r);
void placeShips(Board& board, std::vector<Ship>& ships, int shipNum, std::vector<Solution>& solutions, bool find_all_solutions);

//==========================================================================
//==========================================================================
//read input

void readInput(std::ifstream& infile, Board& board, std::vector<Ship>& ships){
	std::string s;
	infile >> s; // s = board
	infile >> s;
	int numrows = std::atoi(s.c_str());
	infile >> s;
	int numcols = std::atoi(s.c_str());
	
	infile >> s; // s = rows
	std::vector<int> rowsmax;
	std::vector<bool> rowunk;
	for(int i = 0; i < numrows; i++){
		infile >> s;
		int rowmax;
		bool unk;
		if(s == "?"){
			rowmax = numcols;
			unk = true;
		} else{
			rowmax = std::atoi(s.c_str());
			unk = false;
		}
		rowsmax.push_back(rowmax);
		rowunk.push_back(unk);
	}
	infile >> s; // s = cols
	std::vector<int> colsmax;
	std::vector<bool> colunk;
	for(int i = 0; i < numcols; i++){
		infile >> s;
		int colmax;
		bool unk;
		if(s == "?"){
			colmax = numrows;
			unk = true;
		} else{
			colmax = std::atoi(s.c_str());
			unk = false;
		}
		colsmax.push_back(colmax);
		colunk.push_back(unk);
	}
	board = Board(numcols, numrows, colsmax, rowsmax, colunk, rowunk);
	/*std::cout << "rowunk: ";
	for(int i = 0; i < rowunk.size(); ++i){
		std::cout << rowunk[i] << " ";
	}
	std::cout << std::endl << "colunk: ";
	for(int i = 0; i < colunk.size(); ++i){
		std::cout << colunk[i] << " ";
	}
	std::cout << std::endl;*/

	ships.clear();
	while( (infile >> s) && (s != "constraint") ){
		Ship tempship(s);
		ships.push_back(tempship);
	}

	while(infile >> s){
		int row = std::atoi(s.c_str());
		infile >> s;
		int col = std::atoi(s.c_str());
		infile >> s;
		char ch = s[0];
		board.addConstraint(col, row, ch);
		infile >> s;
	}

}

//==========================================================================
//==========================================================================
//Solution helpers

void printSolution(std::ostream& ostr, const Solution& sol){
	ostr << "Solution:" << std::endl;
	for(int i = 0; i < sol.ships.size(); i++){
		sol.ships[i].print(ostr);
	}
	sol.board.print(ostr);
	ostr << std::endl;
}

//==========================================================================

bool validSolution(const Solution& sol){
	for(int i = 0; i < sol.ships.size(); ++i){
		const Ship& currShip = sol.ships[i];
		if(currShip.getRow() < 0 || currShip.getRow() >= sol.board.numRows()){ return false; }
		if(currShip.getColumn() < 0 || currShip.getColumn() >= sol.board.numColumns()){ return false; }
		const std::string& orientation = currShip.getOrientation();
		if( (orientation != "horizontal") && (orientation != "vertical") && (currShip.getType() != "submarine") ){ 
			return false;
		}
	}
	if(!sol.board.areLinesFilled()){ return false; }
	return sol.board.areConstraintsSatisfied();
}

//==========================================================================

bool newSolution(const Solution& sol, const std::vector<Solution>& solutions){
	if(solutions.size() == 0){ return true; }

	for(int sol_num = 0; sol_num < solutions.size(); ++sol_num){
		int num_matched = 0;
		const Solution& sol2 = solutions[sol_num];
		for(int i = 0; i < sol.ships.size(); ++i){
			for(int j = 0; j < sol2.ships.size(); ++j){
				if(sol.ships[i] == sol2.ships[j]){
					num_matched++;
					break;
				}
			}
		}
		if(num_matched >= sol.ships.size()){
			return false;
		}
	}
	return true;
}

//==========================================================================
//==========================================================================
//add ships

void placeHelper(Board& board, std::vector<Ship>& ships, int shipNum, std::vector<Solution>& solutions, bool find_all_solutions, int c, int r){
	bool sub = (ships[shipNum].getType() == "submarine");
	if(sub){
		if(board.canPlace(c, r, "", ships[shipNum])){
			board.placeShip(c, r, "", ships[shipNum]);
			placeShips(board, ships, shipNum+1, solutions, find_all_solutions);
			board.removeShip(ships[shipNum]);
		}
	} else{
		if(board.canPlace(c, r, "horizontal", ships[shipNum])){
			board.placeShip(c, r, "horizontal", ships[shipNum]);
			placeShips(board, ships, shipNum+1, solutions, find_all_solutions);
			board.removeShip(ships[shipNum]);
		}

		if(!find_all_solutions && solutions.size() > 0){ return; }

		if(board.canPlace(c, r, "vertical", ships[shipNum])){
			board.placeShip(c, r, "vertical", ships[shipNum]);
			placeShips(board, ships, shipNum+1, solutions, find_all_solutions);
			board.removeShip(ships[shipNum]);
		}
	}
	if(!find_all_solutions && solutions.size() > 0){ return; }
}

//==========================================================================

void placeShips(Board& board, std::vector<Ship>& ships, int shipNum, std::vector<Solution>& solutions, bool find_all_solutions){
	if(!find_all_solutions && solutions.size() > 0){ return; }

	//all ships have been placed
	if(shipNum >= ships.size()){
		Solution sol(board, ships);
		if( validSolution(sol) && newSolution(sol, solutions) ){
			//finds solutions that don't satisfy constraints
			solutions.push_back(sol);
		}
		return;
	}

	bool unknown = (ships[shipNum].getType() == "unknown");
	std::string types[] = {"submarine", "destroyer", "cruiser", "battleship", "carrier", "cargo", "tanker"};

	for(int c = 0; c < board.numColumns(); ++c){
		for(int r = 0; r < board.numRows(); ++r){
			if(unknown){
				for(int i = 6; i >= 0; --i){
					ships[shipNum].setType(types[i]);
					placeHelper(board, ships, shipNum, solutions, find_all_solutions, c, r);
					ships[shipNum].setType("unknown");
					if(!find_all_solutions && solutions.size() > 0){ return; }
				}
			} else{
				placeHelper(board, ships, shipNum, solutions, find_all_solutions, c, r);
				if(!find_all_solutions && solutions.size() > 0){ return; }
			}
		}
	}
}

//==========================================================================
//==========================================================================

int main(int argc, char* argv[]) {
	if(argc != 2 && argc != 3){
		std::cerr << "Enter 1 or 2 arguments after command name:\n" 
				<< "input_filename (find_all_solutions)\n";
		exit(1);
	}
	std::ifstream infile;
	infile.open(argv[1]);
	if (!infile.good()){
		std::cerr << "Can't open" << argv[1] << "to read.\n";
		exit(1);
	}
	bool find_all_solutions = false;
	if(argc == 3){ 
		std::string fas = argv[2];
		find_all_solutions = (fas == "find_all_solutions"); 
	}

	Board board = Board();
	std::vector<Ship> ships;
	std::vector<Solution> solutions;
	readInput(infile, board, ships);

	std::sort(ships.begin(), ships.end(), by_greatest_length);

	placeShips(board, ships, 0, solutions, find_all_solutions);
	
	for(int i = 0; i < solutions.size(); ++i){
		printSolution(std::cout, solutions[i]);
	}
	if(solutions.size() == 0){
		std::cout << "No solutions" << std::endl;
	} else if(find_all_solutions){
		std::cout << "Found " << solutions.size() << " solution(s)" << std::endl;
	}

	return 0;
}

//==========================================================================
//==========================================================================





















