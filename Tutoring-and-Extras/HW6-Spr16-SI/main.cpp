#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include "Battleship.h"

std::vector<std::string> clear_board(std::vector<std::string> &prev_board, int bh, int bw){
	for(int i = 0; i < bh+3; i++){
  		std::string column (bw+3, 'x');
  		for(int j = 0; j < bw+3; j++){
  			if(i == 0){
	  			column[j] = '-';
	  			column[0] = '+';
	  			column[bw+1] = '+';
	  			column[bw+2] = ' ';
	  		}
	  		else if(i == bh + 1){
	  			column[0] = '+';
	  			column[j] = '-';
	  			column[bw + 1] = '+';
	  			column[bw+2] = ' ';
	  		}
	  		else if(i == bh+2){
	  			column[j] = '#';
	  			column[0] = ' ';
	  			column[bw+1] = ' ';
	  			column[bw+2] = ' ';

	  		}
	  		else{
	  			column[0] = '|';
	  			column[bw+1] = '|';
	  			column[bw+2] = '#';
	  		}
  		 prev_board[j] = column; 
  		 return prev_board;		
  		}
  	}
}

int find_ship_size(std::string battleship){
	if(battleship == "submarine"){
		return 1;
	}
	else if(battleship == "destroyer"){
		return 2;
	}
	else if(battleship == "cruiser"){
		return 3;
	}
	else if (battleship == "battleship"){
		return 4;
	}
	else if(battleship == "carrier"){
		return 5;
	}
	else if(battleship == "cargo"){
		return 6;
	}
	else if(battleship == "tanker"){
		return 7;
	}
	else{
		return 0;
		std::cerr << "error" << std::endl;
	}
}


std::vector<std::string> border_marker(std::vector<std::string> board, Battleship b, int row, int col){
	//checks orientation of most recently put down battleship
	if(b.get_orientation() == "horizontal" ){
		//loops through each pixel surrounding that battleship and
		//checks if its an 'x'
		//if it is, it replaces it with the '*' character
		for(int i = row - 1; i < row + 2; i++){
			if(i != row){
				for(int j = -1; j < b.get_size() + 1; j++){
					if(board[i][col + j] == 'x'){
						board[i][col + j] = '*';
					}
					else{
						continue;
					}
				}
			}
			else{
				if(board[i][col-1] == 'x'){
					board[i][col-1] = '*';
				}
				if(board[i][col + b.get_size()] == 'x'){
					board[i][col + b.get_size()] = '*';
				}
			}
		}
	}
	else{
		std::cout << "what?" << std::endl;
	}
		return board;	
}
bool check_board(std::vector<std::string> board, Battleship b, int row_index, int col_index){
	bool fits = false;
	bool check_row = false;
	bool check_cols = false;
	int col_height = board[0].size();

	char row_value = board[row_index][board.size()];
	int r_value = row_value;
	int x_number = std::count(board[row_index].begin(), board[row_index].end(), 'x');
	int star_number = std::count(board[row_index].begin(), board[row_index].end(), '*');
	std::cout << "number of x: " << x_number << " number of star: " << star_number << std::endl;
	if((x_number + star_number) - (col_height - 3) < r_value){
		check_row = true;
		std::cout << check_row << std::endl;
	}
	int total_col = 0;
	int x_col = 0;
	char col_value = board[board.size()-1][col_index];
	std::cout << "col_value: " << col_value << std::endl;
	int c_value = col_value;
	std::cout << "c-value: " << c_value << std::endl;
	for(int x = c_value; x < c_value + b.get_size(); x++){
		std::cout << "column_value: " << x << std::endl;
		for(int i = 1; i < board.size() - 2; i++){
			if(board[i][x] == 'x' || board[i][x] == '*'){
				x_col++;
			}
			if((board.size() - 3) - x_col < c_value){
				total_col++;
			}
		}
	}
	if(total_col == b.get_size()){
		check_cols = true;
	}
	if(check_cols == true && check_row == true){
		fits = true;
	}
	else{
		fits = false;
	}
	return fits;
}

std::vector<std::string> solution_formula(std::vector<Battleship> battleship_vector, Battleship b, std::vector<std::string> board, int c){
	// if(b.get_orientation() == "horizontal"){
		bool orientation = false;
		c++;
		int col_height = board[0].size() - 3;
		std::string replace_str;
		//makes string to replace
		for(int s = 0; s < b.get_size(); s++){
			replace_str += 'x';
		}
		if(c == battleship_vector.size()){
			for(int i = 1; i < board.size() - 2; ++i){
				std::string::size_type found = board[i].find(replace_str);
				if (found > col_height){
					continue;
				}
				else{
					board[i].replace(found, b.get_size(), b.get_ship_string());
					board = border_marker(board, b, i, found);
					orientation = true;
					// shows board after last change
					std::cout << std::endl;
					std::cout << "AFTER REPLACE last" << std::endl;
					for(int q = 0; q < board.size(); q++){
						std::cout << board[q] << std::endl;
					}
					std::cout << std::endl;
					//end of test code
					std::cout << b.get_name() << ' ' << i << ' ' << found << ' ' << b.get_orientation() << std::endl;
					break;
				}
			}
			return board;			
		}
		else{
			for(int i = 1; i < board.size() - 2; ++i){
				std::string::size_type found = board[i].find(replace_str);
				if (found > col_height){
					continue;
				}
				else{
					// if(!check_board(board, b, i, found)){
					// 	continue;
					// }
					// else{
						board[i].replace(found, b.get_size(), b.get_ship_string());
						board = border_marker(board, b, i, found);
						orientation = true;
						//shows board after a change
						std::cout << std::endl;
						std::cout << "AFTER REPLACE" << std::endl;
						for(int q = 0; q < board.size(); q++){
							std::cout << board[q] << std::endl;
						}
						std::cout << std::endl;
						//end of test code
						//calls the function again with the next battleship
						board = solution_formula(battleship_vector, battleship_vector[c], board, c);
						//print statement
						std::cout << b.get_name() << ' ' << i << ' ' << found << ' ' << b.get_orientation() << std::endl;
						return board;
					}	
				}
			}
			if(orientation == false){
				c--;
				b.change_orientation();
				board = solution_formula(battleship_vector, battleship_vector[c], board, c);
			}
		// }
	// }
	// else{
	// 	c++;
	// 	int x_check = 0;
	// 	bool orientation = false;
	// 	int col_height = board[0].size() - 3;
	// 	for(int k = 1; k < col_height; k++){
	// 		for(int i = 1; i < board.size() - (2 + b.get_size()); ++i){
	// 			if(board[i][k] == 'x'){
	// 				x_check++;
	// 			}
	// 			else{
	// 				x_check = 0;
	// 			}
	// 			if(x_check == b.get_size()){
	// 				for(int y = i; y > i - x_check ; --y){
	// 					orientation = true;
	// 					board[y][k] = b.get_ship_string()[y];
	// 				}
	// 			}
	// 		}
	// 	}
	// }	
}

int main(int argc, char* argv[]) {
	  // Check the number of arguments.
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " in-file out-file\n";
    return 1;
  }
  // Open and test the input file.
  std::ifstream istr(argv[1]);
  if (!istr) {
    std::cerr << "Could not open " << argv[1] << " to read\n";
    return 1;
  }
  // Open and test the output file.
  // std::ofstream ostr(argv[2]);
  // if (!ostr) {
  //   std::cerr << "Could not open " << argv[2] << " to write\n";
  //   return 1;
  // }

  //read in txt file to a vector of lines of the txt file
  std::string c;
  std::vector<std::string> filevector;
  while ( getline(istr,c) ){
  	filevector.push_back(c);
  	// std::cout << c << std::endl;
  }
  //initialize variables
  std::string num1;
  std::string num2;
  std::string term;
  std::vector<std::string> board_dimensions_vector;
  //stringstream the string word by word into a vector of variables
  std::istringstream iss(filevector[0]);
  while(iss >> term){
  	if(term == "board"){
  		continue;
  	}
  	else{
  		board_dimensions_vector.push_back(term);
  	}
  }
  //set the height and width variables
  int board_width = atoi(board_dimensions_vector[1].c_str());
  int board_height = atoi(board_dimensions_vector[0].c_str());
  //make board
  std::vector<std::string> game_board;
  for(int i = 0; i < board_height+3; i++){
  	std::string column (board_width+3, 'x');
  	for(int j = 0; j < board_width+3; j++){
  		if(i == 0){
  			column[j] = '-';
  			column[0] = '+';
  			column[board_width+1] = '+';
  			column[board_width+2] = ' ';
  		}
  		else if(i == board_height + 1){
  			column[0] = '+';
  			column[j] = '-';
  			column[board_width + 1] = '+';
  			column[board_width+2] = ' ';
  		}
  		else if(i == board_height+2){
  			column[j] = '#';
  			column[0] = ' ';
  			column[board_width+1] = ' ';
  			column[board_width+2] = ' ';

  		}
  		else{
  			column[0] = '|';
  			column[board_width+1] = '|';
  			column[board_width+2] = '#';
  		}
  		  		
  	}
  	game_board.push_back(column);
  }
  // gets board stats and puts values in board variable
  std::vector<std::string> rows_vector;
  std::istringstream iss1(filevector[1]);
  while(iss1 >> term){
  	if(term == "rows"){
  		continue;
  	}
  	else{
  		rows_vector.push_back(term);
  	}
  }
  for(int k = 1; k < board_height+1 ; k++){
  	game_board[k][board_height+3] = rows_vector[k-1][0];
  } 
  // gets board_width stats and puts values in board variable
  std::vector<std::string> width_vector;
  std::istringstream iss2(filevector[2]);
  while(iss2 >> term){
  	if(term == "cols"){
  		continue;
  	}
  	else{
  		width_vector.push_back(term);
  	}
  }
  for(int l = 1; l < board_width+1; l++){
  	game_board[board_width+1][l] = width_vector[l-1][0];
  }
  // prints empty game_board
  std::cout << " START BOARD " << std::endl;
  for(int x = 0; x < board_width+2; x++){
  	std::cout << game_board[x] << std::endl;
  }
  std::cout << std::endl;
  //finds the battleships and makes class objects
  std::vector<Battleship> battleships;
  for(int i = 3; i < filevector.size(); i++){
  	if(filevector[i].substr(0 ,9) == "constraint"){
  		break;
  	}
  	else{
	  	int size = find_ship_size(filevector[i]);
	  	Battleship b(filevector[i], size);
	  	battleships.push_back(b);  		
  	}
  }
  //checks list
  for(int l = 0; l < battleships.size(); l++){
  	std::cout << battleships[l].get_name() << std::endl;
  }
  //recursive function designed to loop through each board possibility
  std::cout << "Solution:" << std::endl;
  game_board = solution_formula(battleships, battleships[0], game_board, 0);

  for(int x = 0; x < game_board.size(); ++x){
  	std::cout << game_board[x] << std::endl;
	}


  return 0;
}