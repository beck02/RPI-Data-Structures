#include <cstddef>
#include <iostream>
#include <algorithm>
#include "tetris.h"

// ======================================================================
// Constructor + destructor
// ======================================================================
Tetris::Tetris(int w){
	width = w;
	heights = new int[width];
	data = new char*[width];
	for(int i = 0; i < width; i++){
		heights[i] = 0;
		data[i] = NULL;
	}
}

void Tetris::destroy(){
	for(int i = 0; i < width; i++){
		delete [] data[i];
	}
	delete [] data;
	delete [] heights;
}

// ======================================================================
// Accessors
// ======================================================================
int Tetris::get_max_height() const {
	int max = 0;
	for(int i = 0; i < width; i++){
		if(heights[i] > max){
			max = heights[i];
		}
	}
	return max;
}

int Tetris::count_squares() const{
	int count = 0;
	for(int i = 0; i < width; i++){
		for(int j = 0; j < heights[i]; j++){
			char c = data[i][j];
			if( c == 'I' || c == 'O' || c == 'T' || c == 'Z' || c == 'S' || c == 'L' || c == 'J' ){ // or != '\0'
				count++;
			}
		}
	}
	return count;
}

// ======================================================================
// Mutators
// ======================================================================
void Tetris::copy_col_data(char* dest, int colnum, int top){
	for(int r = 0; r < heights[colnum]; r++){
		dest[r] = data[colnum][r];
	}
	for(int r = heights[colnum]; r < top; r++){
		dest[r] = ' ';
	}
}

// ======================================================================
void Tetris::add_piece(char shape, int rotation, int leftedge){
//shape options: 'I', 'O', 'T', 'Z', 'S', 'L', 'J'
	int nheight; int bottomheight;
	char* ncol1; char* ncol2;
	char* ncol3; char* ncol4;
	if(shape == 'O'){
		//find height of the bottom edge of the new piece
		bottomheight = std::max(heights[leftedge], heights[leftedge+1]);
		nheight = bottomheight + 2;
		//create new column(s)
		ncol1 = new char[nheight];
		ncol2 = new char[nheight];
		//copy old data to new column(s)
		copy_col_data(ncol1, leftedge, bottomheight);
		copy_col_data(ncol2, leftedge+1, bottomheight);
		//put new piece into new column(s)
		ncol1[bottomheight] = 'O';
		ncol1[bottomheight+1] = 'O';
		ncol2[bottomheight] = 'O';
		ncol2[bottomheight+1] = 'O';
		//delete old column(s) and put new column(s) into data
		delete [] data[leftedge];
		delete [] data[leftedge+1];
		data[leftedge] = ncol1;
		data[leftedge+1] = ncol2;
		//update heights
		heights[leftedge] = nheight;
		heights[leftedge+1] = nheight;
	} // end add O 
	else if(shape == 'Z'){ //add Z
		if(rotation == 0 || rotation == 180){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge]-1, std::max(heights[leftedge+1], heights[leftedge+2]));
			nheight = bottomheight + 2;
			//create new column(s)
			ncol1 = new char[nheight];
			ncol2 = new char[nheight];
			ncol3 = new char[nheight-1];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight+1);
			copy_col_data(ncol2, leftedge+1, bottomheight);
			copy_col_data(ncol3, leftedge+2, bottomheight);
			//put new piece into new column(s)
			ncol1[bottomheight+1] = 'Z';
			ncol2[bottomheight+1] = 'Z';
			ncol2[bottomheight] = 'Z';
			ncol3[bottomheight] = 'Z';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			delete [] data[leftedge+2];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			data[leftedge+2] = ncol3;
			//update heights
			heights[leftedge] = nheight;
			heights[leftedge+1] = nheight;
			heights[leftedge+2] = nheight-1;
		} 
		else if(rotation == 90 || rotation == 270){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge], heights[leftedge+1]-1);
			nheight = bottomheight + 2;
			//create new column(s)
			ncol1 = new char[nheight];
			ncol2 = new char[nheight+1];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight);
			copy_col_data(ncol2, leftedge+1, bottomheight+1);
			//put new piece into new column(s)
			ncol1[bottomheight] = 'Z';
			ncol1[bottomheight+1] = 'Z';
			ncol2[bottomheight+1] = 'Z';
			ncol2[bottomheight+2] = 'Z';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			//update heights
			heights[leftedge] = nheight;
			heights[leftedge+1] = nheight+1;
		}
	} // end add Z
	else if(shape == 'S'){ //add S
		if(rotation == 0 || rotation == 180){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge], std::max(heights[leftedge+1], heights[leftedge+2]-1));
			nheight = bottomheight + 2;
			//create new column(s)
			ncol1 = new char[nheight-1];
			ncol2 = new char[nheight];
			ncol3 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight);
			copy_col_data(ncol2, leftedge+1, bottomheight);
			copy_col_data(ncol3, leftedge+2, bottomheight+1);
			//put new piece into new column(s)
			ncol1[bottomheight] = 'S';
			ncol2[bottomheight] = 'S';
			ncol2[bottomheight+1] = 'S';
			ncol3[bottomheight+1] = 'S';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			delete [] data[leftedge+2];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			data[leftedge+2] = ncol3;
			//update heights
			heights[leftedge] = nheight-1;
			heights[leftedge+1] = nheight;
			heights[leftedge+2] = nheight;
		} 
		else if(rotation == 90 || rotation == 270){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge]-1, heights[leftedge+1]);
			nheight = bottomheight + 2;
			//create new column(s)
			ncol1 = new char[nheight+1];
			ncol2 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight+1);
			copy_col_data(ncol2, leftedge+1, bottomheight);
			//put new piece into new column(s)
			ncol1[bottomheight+1] = 'S';
			ncol1[bottomheight+2] = 'S';
			ncol2[bottomheight] = 'S';
			ncol2[bottomheight+1] = 'S';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			//update heights
			heights[leftedge] = nheight+1;
			heights[leftedge+1] = nheight;
		}
	} // end add S
	else if(shape == 'I'){ //add I
		if(rotation == 0 || rotation == 180){
			//find height of the bottom edge of the new piece
			bottomheight = heights[leftedge];
			nheight = bottomheight + 4;
			//create new column(s)
			ncol1 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight);
			//put new piece into new column(s)
			ncol1[bottomheight] = 'I';
			ncol1[bottomheight+1] = 'I';
			ncol1[bottomheight+2] = 'I';
			ncol1[bottomheight+3] = 'I';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			data[leftedge] = ncol1;
			//update heights
			heights[leftedge] = nheight;
		}
		else if(rotation == 90 || rotation == 270){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge], std::max(heights[leftedge+1],std::max(heights[leftedge+2], heights[leftedge+3])));
			nheight = bottomheight + 1;
			//create new column(s)
			ncol1 = new char[nheight];
			ncol2 = new char[nheight];
			ncol3 = new char[nheight];
			ncol4 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight);
			copy_col_data(ncol2, leftedge+1, bottomheight);
			copy_col_data(ncol3, leftedge+2, bottomheight);
			copy_col_data(ncol4, leftedge+3, bottomheight);
			//put new piece into new column(s)
			ncol1[bottomheight] = 'I';
			ncol2[bottomheight] = 'I';
			ncol3[bottomheight] = 'I';
			ncol4[bottomheight] = 'I';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			delete [] data[leftedge+2];
			delete [] data[leftedge+3];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			data[leftedge+2] = ncol3;
			data[leftedge+3] = ncol4;
			//update heights
			heights[leftedge] = nheight;
			heights[leftedge+1] = nheight;
			heights[leftedge+2] = nheight;
			heights[leftedge+3] = nheight;
		}
	} // end add I
	else if(shape == 'T'){ //add T
		if(rotation == 0){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge]-1, std::max(heights[leftedge+1], heights[leftedge+2]-1));
			nheight = bottomheight + 2;
			//create new column(s)
			ncol1 = new char[nheight];
			ncol2 = new char[nheight];
			ncol3 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight+1);
			copy_col_data(ncol2, leftedge+1, bottomheight);
			copy_col_data(ncol3, leftedge+2, bottomheight+1);
			//put new piece into new column(s)
			ncol1[bottomheight+1] = 'T';
			ncol2[bottomheight+1] = 'T';
			ncol2[bottomheight] = 'T';
			ncol3[bottomheight+1] = 'T';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			delete [] data[leftedge+2];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			data[leftedge+2] = ncol3;
			//update heights
			heights[leftedge] = nheight;
			heights[leftedge+1] = nheight;
			heights[leftedge+2] = nheight;
		} 
		else if(rotation == 90){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge]-1, heights[leftedge+1]);
			nheight = bottomheight + 2;
			//create new column(s)
			ncol1 = new char[nheight];
			ncol2 = new char[nheight+1];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight+1);
			copy_col_data(ncol2, leftedge+1, bottomheight);
			//put new piece into new column(s)
			ncol1[bottomheight+1] = 'T';
			ncol2[bottomheight] = 'T';
			ncol2[bottomheight+1] = 'T';
			ncol2[bottomheight+2] = 'T';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			//update heights
			heights[leftedge] = nheight;
			heights[leftedge+1] = nheight+1;
		} 
		else if(rotation == 180){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge], std::max(heights[leftedge+1], heights[leftedge+2]));
			nheight = bottomheight + 1;
			//create new column(s)
			ncol1 = new char[nheight];
			ncol2 = new char[nheight+1];
			ncol3 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight);
			copy_col_data(ncol2, leftedge+1, bottomheight);
			copy_col_data(ncol3, leftedge+2, bottomheight);
			//put new piece into new column(s)
			ncol1[bottomheight] = 'T';
			ncol2[bottomheight+1] = 'T';
			ncol2[bottomheight] = 'T';
			ncol3[bottomheight] = 'T';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			delete [] data[leftedge+2];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			data[leftedge+2] = ncol3;
			//update heights
			heights[leftedge] = nheight;
			heights[leftedge+1] = nheight+1;
			heights[leftedge+2] = nheight;
		} 
		else if(rotation == 270){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge], heights[leftedge+1]-1);
			nheight = bottomheight + 2;
			//create new column(s)
			ncol1 = new char[nheight+1];
			ncol2 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight);
			copy_col_data(ncol2, leftedge+1, bottomheight+1);
			//put new piece into new column(s)
			ncol1[bottomheight] = 'T';
			ncol1[bottomheight+1] = 'T';
			ncol1[bottomheight+2] = 'T';
			ncol2[bottomheight+1] = 'T';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			//update heights
			heights[leftedge] = nheight+1;
			heights[leftedge+1] = nheight;
		}
	} // end add T
	else if(shape == 'L'){ //add L
		if(rotation == 0){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge], heights[leftedge+1]);
			nheight = bottomheight + 1;
			//create new column(s)
			ncol1 = new char[nheight+2];
			ncol2 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight);
			copy_col_data(ncol2, leftedge+1, bottomheight);
			//put new piece into new column(s)
			ncol1[bottomheight] = 'L';
			ncol1[bottomheight+1] = 'L';
			ncol1[bottomheight+2] = 'L';
			ncol2[bottomheight] = 'L';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			//update heights
			heights[leftedge] = nheight+2;
			heights[leftedge+1] = nheight;
		} 
		else if(rotation == 90){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge], std::max(heights[leftedge+1]-1, heights[leftedge+2]-1));
			nheight = bottomheight + 2;
			//create new column(s)
			ncol1 = new char[nheight];
			ncol2 = new char[nheight];
			ncol3 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight);
			copy_col_data(ncol2, leftedge+1, bottomheight+1);
			copy_col_data(ncol3, leftedge+2, bottomheight+1);
			//put new piece into new column(s)
			ncol1[bottomheight] = 'L';
			ncol1[bottomheight+1] = 'L';
			ncol2[bottomheight+1] = 'L';
			ncol3[bottomheight+1] = 'L';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			delete [] data[leftedge+2];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			data[leftedge+2] = ncol3;
			//update heights
			heights[leftedge] = nheight;
			heights[leftedge+1] = nheight;
			heights[leftedge+2] = nheight;
		} 
		else if(rotation == 180){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge]-2, heights[leftedge+1]);
			nheight = bottomheight + 3;
			//create new column(s)
			ncol1 = new char[nheight];
			ncol2 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight+2);
			copy_col_data(ncol2, leftedge+1, bottomheight);
			//put new piece into new column(s)
			ncol1[bottomheight+2] = 'L';
			ncol2[bottomheight] = 'L';
			ncol2[bottomheight+1] = 'L';
			ncol2[bottomheight+2] = 'L';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			//update heights
			heights[leftedge] = nheight;
			heights[leftedge+1] = nheight;
		} 
		else if(rotation == 270){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge], std::max(heights[leftedge+1], heights[leftedge+2]));
			nheight = bottomheight + 1;
			//create new column(s)
			ncol1 = new char[nheight];
			ncol2 = new char[nheight];
			ncol3 = new char[nheight+1];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight);
			copy_col_data(ncol2, leftedge+1, bottomheight);
			copy_col_data(ncol3, leftedge+2, bottomheight);
			//put new piece into new column(s)
			ncol1[bottomheight] = 'L';
			ncol2[bottomheight] = 'L';
			ncol3[bottomheight] = 'L';
			ncol3[bottomheight+1] = 'L';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			delete [] data[leftedge+2];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			data[leftedge+2] = ncol3;
			//update heights
			heights[leftedge] = nheight;
			heights[leftedge+1] = nheight;
			heights[leftedge+2] = nheight+1;
		}
	} // end add L
	else if(shape == 'J'){ //add J
		if(rotation == 0){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge], heights[leftedge+1]);
			nheight = bottomheight + 1;
			//create new column(s)
			ncol1 = new char[nheight];
			ncol2 = new char[nheight+2];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight);
			copy_col_data(ncol2, leftedge+1, bottomheight);
			//put new piece into new column(s)
			ncol1[bottomheight] = 'J';
			ncol2[bottomheight] = 'J';
			ncol2[bottomheight+1] = 'J';
			ncol2[bottomheight+2] = 'J';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			//update heights
			heights[leftedge] = nheight;
			heights[leftedge+1] = nheight+2;
		} 
		else if(rotation == 90){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge], std::max(heights[leftedge+1], heights[leftedge+2]));
			nheight = bottomheight + 1;
			//create new column(s)
			ncol1 = new char[nheight+1];
			ncol2 = new char[nheight];
			ncol3 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight);
			copy_col_data(ncol2, leftedge+1, bottomheight);
			copy_col_data(ncol3, leftedge+2, bottomheight);
			//put new piece into new column(s)
			ncol1[bottomheight] = 'J';
			ncol1[bottomheight+1] = 'J';
			ncol2[bottomheight] = 'J';
			ncol3[bottomheight] = 'J';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			delete [] data[leftedge+2];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			data[leftedge+2] = ncol3;
			//update heights
			heights[leftedge] = nheight+1;
			heights[leftedge+1] = nheight;
			heights[leftedge+2] = nheight;
		} 
		else if(rotation == 180){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge], heights[leftedge+1]-2);
			nheight = bottomheight + 3;
			//create new column(s)
			ncol1 = new char[nheight];
			ncol2 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight);
			copy_col_data(ncol2, leftedge+1, bottomheight+2);
			//put new piece into new column(s)
			ncol1[bottomheight] = 'J';
			ncol1[bottomheight+1] = 'J';
			ncol1[bottomheight+2] = 'J';
			ncol2[bottomheight+2] = 'J';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			//update heights
			heights[leftedge] = nheight;
			heights[leftedge+1] = nheight;
		} 
		else if(rotation == 270){
			//find height of the bottom edge of the new piece
			bottomheight = std::max(heights[leftedge]-1, std::max(heights[leftedge+1]-1, heights[leftedge+2]));
			nheight = bottomheight + 2;
			//create new column(s)
			ncol1 = new char[nheight];
			ncol2 = new char[nheight];
			ncol3 = new char[nheight];
			//copy old data to new column(s)
			copy_col_data(ncol1, leftedge, bottomheight+1);
			copy_col_data(ncol2, leftedge+1, bottomheight+1);
			copy_col_data(ncol3, leftedge+2, bottomheight);
			//put new piece into new column(s)
			ncol1[bottomheight+1] = 'J';
			ncol2[bottomheight+1] = 'J';
			ncol3[bottomheight] = 'J';
			ncol3[bottomheight+1] = 'J';
			//delete old column(s) and put new column(s) into data
			delete [] data[leftedge];
			delete [] data[leftedge+1];
			delete [] data[leftedge+2];
			data[leftedge] = ncol1;
			data[leftedge+1] = ncol2;
			data[leftedge+2] = ncol3;
			//update heights
			heights[leftedge] = nheight;
			heights[leftedge+1] = nheight;
			heights[leftedge+2] = nheight;
		}
	} // end add J
}



// ======================================================================

int Tetris::remove_full_rows(){
	for(int i = 0; i < width; i++){
		if(heights[i] == 0) { return 0; }
	}
	int rowsremoved = 0;
	int max = get_max_height();
	for(int h = 0; h < max; h++){
		bool full = true;
		for(int i = 0; i < width; i++){
			if(heights[i] <= h){ full = false; break; }
			if(data[i][h] == ' '){ full = false; break; }
		}
		if(full){
			remove_row(h);
			rowsremoved++;
			h--;
			max--;
		}
	}
	return rowsremoved;
}
// ======================================================================
void Tetris::remove_row(int h){
	// std::cout << "Removing row " << h << std::endl;
	for(int i = 0; i < width; i++){
		char* temp = new char[heights[i]-1];
		bool hasdata = false;
		//copy data from below the removed row
		for(int j = 0; j < h; j++){
			temp[j] = data[i][j];
			if(temp[j] != ' '){ hasdata = true; }
		}
		//copy data from above the removed row
		for(int j = h+1; j < heights[i]; j++){
			temp[j-1] = data[i][j];
			if(temp[j-1] != ' '){ hasdata = true; }
		}
		delete [] data[i];
		/*for(int j = 0; j < heights[i]-1; j++){
			if(temp[j] != '.'){ hasdata = true; break; }
		}*/
		if(hasdata){
			data[i] = temp;
			heights[i]--;
		} else{
			delete [] temp;
			data[i] = NULL;
			heights[i] = 0;
		}
	}
}

// ======================================================================

void Tetris::add_left_column(){
	width++;
	int* nhgt = new int[width];
	char** ndat = new char*[width];
	nhgt[0] = 0;
	ndat[0] = NULL;
	for(int i = 1; i < width; i++){
		ndat[i] = data[i-1];
		nhgt[i] = heights[i-1];
	}
	delete [] heights;
	heights = nhgt;
	delete [] data;
	data = ndat;
}

// ======================================================================

void Tetris::add_right_column(){
	width++;
	int* nhgt = new int[width];
	char** ndat = new char*[width];
	nhgt[width-1] = 0;
	ndat[width-1] = NULL;
	for(int i = 0; i < width-1; i++){
		ndat[i] = data[i];
		nhgt[i] = heights[i];
	}
	delete [] heights;
	heights = nhgt;
	delete [] data;
	data = ndat;
}

// ======================================================================

void Tetris::remove_left_column(){
	width--;
	int* nhgt = new int[width];
	char** ndat = new char*[width];
	for(int i = 0; i < width; i++){
		ndat[i] = data[i+1];
		nhgt[i] = heights[i+1];
	}
	delete [] heights;
	heights = nhgt;
	delete [] data[0];
	delete [] data;
	data = ndat;
}

// ======================================================================

void Tetris::remove_right_column(){
	width--;
	int* nhgt = new int[width];
	char** ndat = new char*[width];
	for(int i = 0; i < width; i++){
		ndat[i] = data[i];
		nhgt[i] = heights[i];
	}
	delete [] heights;
	heights = nhgt;
	delete [] data[width];
	delete [] data;
	data = ndat;
}

// ======================================================================
// ======================================================================








