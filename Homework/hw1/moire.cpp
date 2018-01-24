/**
File: moire.cpp
Purpose: Create ascii art from command line input.
**/

#include <iostream>
#include <fstream>
#include <cstdlib>

void drawSquare(int size, char* patt, char* file){
	std::ofstream out_str(file);
	if (!out_str.good()) {
		std::cerr << "Can't open " << file << " to write.\n";
		exit(1);
	}
	
	for(int i = 0; i < size; i++){ //first line
		out_str << "*";
	}
	out_str << std::endl;
	
	std::string p = patt;
	int pIndex = 0;
	for(int lineNum = 2; lineNum < size; lineNum++){ //each line
		out_str << "*";
		for(int charNum = 2; charNum < size; charNum++){ //each char in line
			if( pIndex >= p.length() ){
				pIndex = 0;
			}
			out_str << patt[pIndex];
			pIndex++;
		}
		out_str << "*" << std::endl;
	}
	
	for(int i = 0; i < size; i++){ //last line
		out_str << "*";
	}
	out_str << std::endl;
}

void drawRightTriangle(int size, char* patt, char* file){
	std::ofstream out_str(file);
	if (!out_str.good()) {
		std::cerr << "Can't open " << file << " to write.\n";
		exit(1);
	}
	
	out_str << "*" << std::endl << "**" << std::endl; //first two lines
	
	std::string p = patt;
	int pIndex = 0;
	for(int lineNum = 3; lineNum < size; lineNum++){ //each line
		out_str << "*";
		for(int charNum = 2; charNum < lineNum; charNum++){ //each char in line
			if( pIndex >= p.length() ){
				pIndex = 0;
			}
			out_str << patt[pIndex];
			pIndex++;
		}
		out_str << "*" << std::endl;
	}
	
	for(int i = 0; i < size; i++){ //last line
		out_str << "*";
	}
	out_str << std::endl;
} 

void drawIsosTriangle(int size, char* patt, char* file){
	std::ofstream out_str(file);
	if (!out_str.good()) {
		std::cerr << "Can't open " << file << " to write.\n";
		exit(1);
	}
	
	for(int i = 0; i < size-1; i++){ //first line
		out_str << " ";
	}
	out_str << "*" << std::endl;
	
	std::string p = patt;
	int pIndex = 0;
	for(int lineNum = 2; lineNum < size; lineNum++){ //for each line
		for(int i = 0; i < size-lineNum; i++){
			out_str << " ";
		}
		out_str << "*";
		for(int charNum = 0; charNum < (1 + 2*(lineNum-2) ); charNum++){ //each char in line
			if( pIndex >= p.length() ){
				pIndex = 0;
			}
			out_str << patt[pIndex];
			pIndex++;
		}
		out_str << "*" << std::endl;
	}
	
	for(int i = 0; i < ((2*size) - 1); i++){ //last line
		out_str << "*";
	}
	out_str << std::endl;
} 

void drawDiamond(int size, char* patt, char* file){
	std::ofstream out_str(file);
	if (!out_str.good()) {
		std::cerr << "Can't open " << file << " to write.\n";
		exit(1);
	}
	
	for(int i = 0; i < (size/2); i++){ //first line
		out_str << " ";
	}
	out_str << "*" << std::endl;
	
	int lineNum = 0, charNum = 0;
	
	std::string p = patt;
	int pIndex = 0;
	for(lineNum = 2; lineNum <= size/2; lineNum++){ //for each line on top half
		for(int i = 0; i < (size/2)-lineNum+1; i++){
			out_str << " ";
		}
		out_str << "*";
		for(charNum = 0; charNum < (1 + 2*(lineNum-2) ); charNum++){ //each char in line
			if( pIndex >= p.length() ){
				pIndex = 0;
			}
			out_str << patt[pIndex];
			pIndex++;
		}
		out_str << "*" << std::endl;
	}
	
	if( (size%2) != 0 ){ //if the height is odd, and therefore should have a middle line
		out_str << "*";
		for(int charNum = 2; charNum < size; charNum++){
			if( pIndex >= p.length() ){
				pIndex = 0;
			}
			out_str << patt[pIndex];
			pIndex++;
		}
		out_str << "*" << std::endl;
	}
	
	int lEnd = 0, cEnd = 0;
	if( (size%2) == 0 ){
		lEnd = size;
	} else if( (size%2) != 0 ){
		lEnd = size-1;
	}
	for(lineNum = (size/2)+1; lineNum < lEnd; lineNum++){ //for each line on bottom half
		for(int i = 0; i < lineNum-(size/2); i++){
			out_str << " ";
		}
		out_str << "*";
		if( (size%2) == 0 ){
			cEnd = ( (size-1) - 2*(lineNum-(size/2)) );
		} else if( (size%2) != 0 ){
			cEnd = ( (size-2) - 2*(lineNum-(size/2)) );
		}
		for(charNum = 0; charNum < cEnd; charNum++){ //each char in line
			if( pIndex >= p.length() ){
				pIndex = 0;
			}
			out_str << patt[pIndex];
			pIndex++;
		}
		out_str << "*" << std::endl;
	}
	
	for(int i = 0; i < (size/2); i++){ //last line
		out_str << " ";
	}
	out_str << "*" << std::endl;
}

int main( int argc, char* argv[] ){
	if( argc != 5 ){
		std::cerr << "Please enter four arguments after the command name: \n" 
			<< "(pattern string) (int height) (shape type string) (output filename string)\n"
			<< "Shape type should be one of: square, right_triangle, isosceles_triangle, diamond\n" 
			<< "If including special chars for pattern, put them in single quotation marks (' ')" << std::endl;
			return 1;
	}
	//1=pattern string, 2=height, 3=shape, 4=filename
	std::string strH = argv[2];
	int height = atoi(strH.c_str());
	std::string shape = argv[3];
	
	if( shape == "square" ){
		drawSquare(height, argv[1], argv[4]);
	} else if( shape == "right_triangle" ){
		drawRightTriangle(height, argv[1], argv[4]);
	} else if( shape == "isosceles_triangle" ){
		drawIsosTriangle(height, argv[1], argv[4]);
	} else if( shape == "diamond" ){
		drawDiamond(height, argv[1], argv[4]);
	} else{
		std::cerr << "Invalid shape type. Valid shape types are:\n"
					<< "square, right_triangle, isosceles_triangle" << std::endl;
		return 1;
	}
	
	return 0;
}


