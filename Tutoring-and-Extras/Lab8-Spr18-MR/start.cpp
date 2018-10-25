// Starting code for Checkpoints 2 and 3.  This includes
// functions to read the grid and to output it.

#include <fstream>
#include <iostream>
#include <list>
#include <vector>
using namespace std;


// A simple class to represent a point location.  It only has a
// constructor and a two public member variables.  This is one of the
// few times that you are allowed to use non-private member variables.

class Point {
public:
	Point(int x0, int y0) : x(x0), y(y0) {}
	int x,y;
};


// Input the grid and the start location.  The input is a sequence of
// x y locations, terminated by x==0 and y==0.  The last input, which
// follows 0 0 input, is the start location.
//
// The grid is represented as a 2d vector of bools, with each location
// that is blocked --- meaning that no path can go through --- being
// represented by the value "true".  The grid is large enough to
// include all blocked points and include the starting location.  The
// first coordinate of the vector of vectors is the x coordinate, and
// the second is the y coordinate.  The format of the input is
// specified in the lab handout.

void read_grid(istream& istr, vector<vector<bool> > & blocked_grid,
				 int & start_x, int & start_y) {

	// Read the x y locations into a list of Points.  Keep track of the
	// max x and max y values so that the size of the grid can be
	// determined.
	int x, y;
	int max_x = 0, max_y = 0;  // keep track of the max coordinate values
	list<Point> blocked_points;
	while ((istr >> x >> y) && ! (x==0 && y==0)) {
		blocked_points.push_back(Point(x,y));
		if (x > max_x) max_x = x;
		if (y > max_y) max_y = y;
	}

	// Now that a 0 0 location has been read, read the start location.
	// If this is beyond the max x or y value then update these values.
	istr >> start_x >> start_y;
	if (start_x > max_x) max_x = start_x;
	if (start_y > max_y) max_y = start_y;

	// Make a vector of vectors with all entries marked false.
	vector<bool> one_row_of_ys(max_y+1, false);
	vector<vector<bool> > empty_grid(max_x+1, one_row_of_ys);
	blocked_grid = empty_grid;

	// For Point in the list, mark the location in the list as blocked.
	list<Point>::iterator p;
	for (p = blocked_points.begin(); p != blocked_points.end(); ++p) {
		blocked_grid[p->x][p->y] = true;
	}
}


// Output the grid to cout.  The form of the output is explained in
// the cout statement below.

/*THIS IS WHAT I WAS WORKING ON BUT IT DIDN'T WORK SO I COMMENTED IT OUT AND
TRIED AGAIN IN THE FUNCTION BELOW */

// void print_grid(vector<vector<bool> > const& blocked_grid, int start_x, int start_y) {

// 	cout << "Here is the grid with the origin in the upper left corner, x increasing \n"
// 		 << "horizontally and y increasing down the screen.  An 'X' represents a blocked\n"
// 		 << "location and the 'S' represents the starting location.\n\n";

// 	for (int y=0; y<blocked_grid[0].size(); ++y) {
// 		for (int x=0; x<blocked_grid.size(); ++x) {
// 			if (x == start_x && y == start_y)
// 				cout << " S";
// 			else if (blocked_grid[x][y])
// 				cout << " X";
// 			else
// 				cout << " .";
// 		}
// 		cout << '\n';
// 	}
// }

//============================================================================================================
//============================================================================================================

void sam_print_grid(vector<vector<bool> > const& blocked_grid, int start_x, int start_y, vector<Point> const& path = vector<Point>()) {
	cout << "Here is the grid with the origin in the upper left corner, x increasing \n"
		 << "horizontally and y increasing down the screen.  An 'X' represents a blocked\n"
		 << "location, the 'S' represents the starting location, and a '$' represents\n"
		 << "a step in one of the paths found from S to the origin.\n\n";

	for (int y=0; y<blocked_grid[0].size(); ++y) {
		for (int x=0; x<blocked_grid.size(); ++x) {
			if (x == start_x && y == start_y){
				cout << " S";
			} else if (blocked_grid[x][y]) {
				cout << " X";
			} else{
				bool part_of_path = false;
				for(int i = 0; i < path.size(); ++i){
					if( (path[i].x == x) && (path[i].y == y) ){
						cout << " $";
						part_of_path = true;
						break;
					}
				}
				if(!part_of_path){
					cout << " .";
				}
			}
		}
		cout << '\n';
	}
}

//============================================================================================================

string sam_get_point_string(int x, int y){
	return "(" + to_string(x) + ", " + to_string(y) + ")";
}

string sam_get_point_string(Point point){
	return "(" + to_string(point.x) + ", " + to_string(point.y) + ")";
}

//============================================================================================================

void sam_print_path(vector<Point> const& path){
	string between_points = " -> ";
	string path_string = "Path: ";
	for(int i = 0; i < path.size(); ++i){
		if(i > 0){
			path_string += between_points;
		}
		path_string += sam_get_point_string(path[i]);
	}
	cout << path_string << endl;
}

//============================================================================================================

int sam_paths(int x, int y, const vector<vector<bool> >& blocked_grid, vector<Point> path, bool& printed){
	path.push_back(Point(x, y));
	if ((x==1 && y == 0) || (x ==0 && y ==1)) {
		if(!printed){
			path.push_back(Point(0, 0));
			sam_print_grid(blocked_grid, path[0].x, path[0].y, path);
			sam_print_path(path);
			printed = true;
		}
		return 1;
	}
	else {
		int left = 0;
		int down = 0;
		if( (x-1 >= 0) && (blocked_grid[x-1][y] != true) ) {
			left = sam_paths(x-1, y, blocked_grid, path, printed);
		}
		if( (y-1 >= 0) && (blocked_grid[x][y-1] != true) ) {
			down = sam_paths(x, y-1, blocked_grid, path, printed);
		}
		return left + down;
	}

}

//============================================================================================================
//============================================================================================================

void print_grid(vector<Point>& path, vector<vector<bool> > const& blocked_grid, int start_x, int start_y) {

	cout << "Here is the grid with the origin in the upper left corner, x increasing \n"
	<< "horizontally and y increasing down the screen.  An 'X' represents a blocked\n"
	<< "location and the 'S' represents the starting location.\n\n";

	for (int y=0; y<blocked_grid[0].size(); ++y) {
		for (int x=0; x<blocked_grid.size(); ++x) {
			if (x == start_x && y == start_y) {
				cout << " S";
			}
			else if (blocked_grid[x][y]) {
				cout << " X";
			}
			else{
				for (int i = 0; i < path.size(); i++) {
					if ((path[i].x && path[i].y) == !blocked_grid[x][y]) {
						cout << " $";
					}
				}
				cout << " .";
			}
		}
		cout << '\n';
	}
}

int paths(int x, int y, const vector<vector<bool> >& vec, vector<Point> &path, bool &occured){
	if ((x==1 && y == 0) || (x ==0 && y ==1)) {
		occured = true;
		std::cout << "(" <<x << ", "<<y << ")"<< '\n';
		return 1;
	}
	else {
		int left = 0;
		int down = 0;
		int current_path = 0;
		if((x-1 >= 0) && (vec[x-1][y] != true)) {
			current_path = paths(x-1,y, vec, path, occured);
			if (occured && current_path == 1) {
				path.push_back(Point(x-1,y));
			}
			else if (occured && current_path == 0) {
				path.push_back(Point(x-1,y));
			}
			left = paths(x-1,y, vec, path, occured);
		}
		if((y-1 >= 0) && (vec[x][y-1] != true)) {
			current_path = paths(x,y-1, vec, path, occured);
			if (occured && current_path == 1) {
				path.push_back(Point(x,y-1));
			}
			else if (occured && current_path == 0) {
				path.push_back(Point(x-1,y));
			}
			down = paths(x,y-1, vec, path, occured);
		}
		return left+down;
	}

}

//============================================================================================================
//============================================================================================================

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " grid-file\n";
		return 1;
	}
	ifstream istr(argv[1]);
	if (!istr) {
		cerr << "Could not open " << argv[1] << endl;
		return 1;
	}
	std::vector<Point> path;
	bool occured = false;

	vector<vector<bool> > blocked_grid;
	int start_x, start_y;
	read_grid(istr, blocked_grid, start_x, start_y);
	print_grid(path, blocked_grid, start_x, start_y);

	// Start here with your code...
	std::cout << "==========Testing recursion==========" << '\n';
	// int with_blocked_paths = paths(start_x, start_y, blocked_grid, path, occured);
	// std::cout << "Number of paths from (" << start_x << ", " << start_y << "): " << with_blocked_paths << std::endl;

	int sam_with_blocked_paths = sam_paths(start_x, start_y, blocked_grid, vector<Point>(), occured);
	std::cout << "Number of paths from " << sam_get_point_string(start_x, start_y) << ": " << sam_with_blocked_paths << std::endl;
	std::cout << "=====================================" << '\n';


	return 0;
}

