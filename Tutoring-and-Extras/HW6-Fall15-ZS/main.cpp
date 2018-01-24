#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "board.h"
#include "ship.h"

//==========================================================================

void placeShips(Board& board, std::vector<Ship>& ships, int shipNum, std::vector<Solution>& solutions, bool find_all_solutions);

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






















