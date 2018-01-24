#ifndef __tetris_h_
#define __tetris_h_

class Tetris{
public:
	// Constructor + destructor
	Tetris(int w);
	void destroy();
	
	// Accessors
	int get_max_height() const;
	int count_squares() const;
	int get_width() const { return width; }
	void print() const;

	// Mutators
	void add_piece(char shape, int rotation, int leftedge);
	int remove_full_rows();
	void add_left_column();
	void add_right_column();
	void remove_left_column();
	void remove_right_column();

private:
	int width;
	int* heights;
	char** data;

	void copy_col_data(char* dest, int colnum, int top);
	void remove_row(int h);
};


#endif