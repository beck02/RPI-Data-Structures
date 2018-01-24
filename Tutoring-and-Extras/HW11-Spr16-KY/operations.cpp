#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <fstream>
#include <cstdio>
#include <cmath>
#include "triangle.h"
#include "operations.h"

//*****************************************************************************
// There ARE errors in this file
//*****************************************************************************
// This file defines
//*****************************************************************************

// I hate typing out "unsigned"
typedef unsigned int uint;

/* Comparing floating point numbers is quite difficult. Because they have a
	 finite precision, evaluating something like (3.2 == 3.2) is not reliably
	 going to return true. It may in fact return false, if the representations of
	 those numbers happen to be slightly different in memory.
	 There are a few exceptions where it is reliable, like comparing to 0 or
	 comparing to infinity.
	 Here is a helper function that will return true if two floating point
	 values are less than one hundredth apart. In this case, they will be
	 considered to be equal.
	 There are NO BUGS in this function */
bool close_enough(double d1, double d2) {
	return (std::abs(d1-d2) < 0.01);
}


/* multidivide: A function to divide a numerator by four different numbers.
	 Converts it to a float to handle the division correctly.
	 Used for the arithmetic operations. */
float multidivide(int numerator, int d1, int d2, int d3, int d4) {
	float f = ((float)((float)((float)((float)numerator / d1) / d2) / d3) / d4);
	return f;
}

int arithmetic_operations() {

	// set up some variables
	int a = 10;
	int b = 46;
	int c = 4;
	int d = c - b;              // -42
	int e = b - 3*a + 4*c;      //  32
	int f = 2*b + 2*c;          //  100
	int g = e - (b/c) + d + 20; // -1
	int h = (f/c) / 8;          //  3
	int m = (d / h) / 7;        // -2
	int n = g + m;              // -3
	int p = (f / e) - c;        // -1
	int q = f + 2*d;            // 16
	int r = g + m + m + n;      // -8
	float s = (float)a / f;            //  0.1

	// 100 / -1 / 4 / 5 / -1 = 5
	std::cout << "Multidivide #1: " << multidivide(f, g, c, 5, g)
			<< " (expected 5)." << std::endl;

	assert(multidivide(f,g,c,5,g) == 5);

	// 840 / -42 / -2 / -1 / 1  = -10
	std::cout << "Multidivide #2: " << multidivide(840, d, m, p, 1)
			<< " (expected -10)." << std::endl;

	assert(multidivide(840, d, m, p, 1) == -10);

	// 320 / -8 / -1 / 10 / -2 = -2
	std::cout << "Multidivide #3: " << multidivide(320, r, p, a, m)
			<< " (expected -2)." << std::endl;

	assert(multidivide(320, r, p, a, m) == -2);

	// -3200 / -1 / 16 / -8 / 5 = -5
	std::cout << "Multidivide #4: " << multidivide((e*f*-1), p, q, r, (a/2))
			<< " (expected -5)." << std::endl;

	assert(multidivide(e*f*-1, p, q, r, (a/2)) == -5);

	// 1000 / 10 / 10 / 10 / 10 = 0.1
	float zeropointone = multidivide(f*10, a, a, a, a);
	std::cout << "Multidivide #5: " << zeropointone
			<< " (expected 0.1)." << std:: endl;

	assert(close_enough(zeropointone, s));

//*****************************************************************************
// The remainder of this function has NO bugs
	std::cout << "Finished the arithmetic operations" << std::endl;
	return (m+r) ^ q ^ ((int) zeropointone) ^ (d*e*f*g*h);
//*****************************************************************************
}


/* Note that this gets passed argc and argv unmodified from main. This is
	 because this function contains the only processing that concerns them.

	 Other note: "char*&" means "reference to a char*". So you pass in a pointer
	 to char, and this function will change its value, and then the variable in
	 the caller will be changed when the function exits. */
bool file_operations(int argc, char** argv, char*& returned_buffer,
				 int& retlen) {

	// Error checking on command line arguments
	if(argc != 2) {
		std::cerr << "Usage: " << argv[0] << " datafile" << std::endl;
		std::cerr << "Couldn't start operations." << std::endl;
		return 1;
	}

	// open a file in binary mode
	// use the first argument passed to the program as the filename
	std::ifstream infile(argv[1], std::ifstream::binary);

	// make sure it's been opened correctly
	if(!infile) {
		std::cerr << "That file could not be opened!" << std::endl;
		return false;
	}
	std::cout << "Successfully opened the input file." << std::endl;

	int length;

	// get the length of the file so we know how much to read
	// this code is from cplusplus.com/reference/istream/istream/read/
	infile.seekg(0, infile.end);
	length = infile.tellg();
	infile.seekg(0, infile.beg);
	
	// make an array of bytes to hold this information
	char* buffer = new char[length];
	
	// can't use streaming I/O (the >> operator) because the file is binary data.
	// Instead, we'll use the .read() function.
	infile.read(buffer, length);

	// make sure all data was read - use gcount() to get the number of bytes
	std::cout << "Successfully read in " << infile.gcount() << " bytes of data."
			<< std::endl;
	assert(infile.gcount() == length);

//*****************************************************************************
// The remainder of this function has NO bugs
	std::cout << "Finished the file operations" << std::endl;
	retlen = length;
	returned_buffer = buffer;
	return true;
//*****************************************************************************
}


/** pythagoras: A function that tests whether two numbers are two parts of a
 *	Pythagorean triple, and if they are, returns the third number.
 *	One of the arguments can be the hypotenuse! Passing this 3 and 5, or 5 and 3,
 *	should have it return 4.
 *	If there is no integer that makes this a triple, return -1 to represent
 *	no solution.
 *	Used in the array operations.
 */
int pythagoras(int x, int y) {
	double* placeholder = new double; // will store the integer part from modf
									  // read up on modf with "man modf" in your terminal

	// x and y are both legs
	float sumsquares = x*x + y*y;
	float fracpart = modf(sqrt(sumsquares), placeholder);
	if(fracpart == 0){
		return (int) *placeholder;
	}
	// x is the hypotenuse, need to subtract instead of add
	float diffsquares = x*x - y*y;
	fracpart = modf(sqrt(diffsquares), placeholder);
	if(fracpart == 0){
		return (int) *placeholder;
	}
	// y is the hypotenuse, need to subtract instead of add
	diffsquares = y*y - x*x;
	fracpart = modf(sqrt(diffsquares), placeholder);
	if(fracpart == 0){
		return (int) *placeholder;
	}
	// no triple exists
	return -1;
}

int array_operations() {
	// what we're doing here is creating and populating a 2D array of ints.
	// We'll use the pythagoras function to store its results for every coordinate
	// pair.
	const int size = 25;
	int** array = new int*[size];
	for(int x = 0; x < size; ++x) {
		array[x] = new int[size];
		for(int y = 0; y < size; ++y) {
			array[x][y] = 0;
		}
	}
	// sanity check
	assert(array[1][1] == 0);

	// store pythagorean numbers in the array
	for(int x = 1; x < size; ++x) {
		for(int y = 1; y < size; ++y) {
			array[x][y] = pythagoras(x, y);
		}
	}
	// do some checks
	assert(array[1][2] == -1); // no triple exists
	assert(array[3][4] == 5);
	assert(array[5][4] == 3);
	assert(array[13][12] == 5);
	assert(array[8][15] == 17);
	assert(array[8][16] != 17);
	assert(array[17][8] == 15);
	assert(array[5][3] == array[3][5]);
	assert(array[7][24] == 25);
	assert(array[12][16] == 20); // 3-4-5 triple times 4
	assert(array[5][15] == -1);
	assert(array[24][7] != -1);

	/* Now iterate over and print the array, using pointers.
		 Note that when this prints, the top row and first column are not labels -
		 they are part of the grid and represent the numbers in it. */
	std::cout << "Printing the Pythagorean numbers array." << std::endl;
	int** tmp_ptr = array;
	for(int x = 1; x < size; ++x, ++tmp_ptr) {
		int* tmp_ptr2 = *tmp_ptr;
		for(int y = 1; y < size; ++y, ++tmp_ptr2) {
			int tmp = *tmp_ptr2;
			// pad single-digit numbers with a space so it looks nice
			// ain't nobody got time for <iomanip>
			std::string maybe_space = ((tmp < 10 && tmp >= 0) ? " " : "");
			std::cout << maybe_space << *tmp_ptr2 << " ";
		}
		std:: cout << std::endl;
	}

//*****************************************************************************
// The remainder of this function has NO bugs
	std::cout << "Finished the array operations" << std::endl;
	int sum = 0;
	for(int x = 5; x < 18; ++x) {
		for(int y = 3; y < 10; ++y) {
			sum += array[x][y];
		}
	}
	return sum;
//*****************************************************************************
}





/* Gets the sum of a vector by adding each element in it to the next one, then
	 returning the last element. inVec will be modified by this function.
	 Used in vector operations. */
int vector_sum(std::vector<int>& inVec) {
	for(uint i = 0; i < inVec.size()-1; ++i) {
		inVec[i+1] = inVec[i] + inVec[i+1];
	}
	return inVec[inVec.size()-1];
}

/* Compares two vectors of ints to each other, element by element.
	 If every number in v1 is strictly greater than the corresponding number in
	 v2, return true; otherwise return false. */
bool vector_compare(const std::vector<int>& v1, const std::vector<int>& v2) {
	bool success = true;
	for(uint i = 0; i < v1.size(); ++i) {
		if(v1[i] < v2[i]) {
			success = false;
		}
	}
	return success;
}

int vector_operations() {

	// create a vector v1 with 7 entries of 25
	std::vector<int> v1(7, 25);
	// create another vector with entries 1-10
	std::vector<int> v2;
	for(uint i = 1; i <= 10; ++i) {
		v2.push_back(i);
	}
	// and one with entries -5 to 5
	std::vector<int> v3;
	for(int i = -5; i <= 5; ++i) {
		v3.push_back(i);
	}
	assert(v3[5] == 0);
	
	int v1sum = vector_sum(v1);
	int v2sum = vector_sum(v2);
	int v3sum = vector_sum(v3);
	assert(v1sum == 175);
	assert(v1[2] == 75);
	assert(v1[5] == 150);
	assert(v2sum == 55);
	assert(v2[2] == 6);
	assert(v3sum == 0);
	assert(v3[10] == 0);
	for(uint i=0; i < v3.size(); ++i) { assert(v3[i] <= 0); }
	int v1sumsum = vector_sum(v1);
	int v2sumsum = vector_sum(v2);
	assert(v1sumsum == 700);
	assert(v1[2] == 150);
	for(uint i=0; i<v1.size(); ++i) { assert(v1[i] != 225); }
	assert(v1[5] == 525);

	int counter = 0;
	for(uint i = 0; i < v2.size(); ++i) {
		// count the number of multiples of 10 in v2
		if(v2[i] % 10 == 0) {
			counter++;
		}
	}
	// there should be 4 of them
	assert(counter == 4);


	// more vectors
	std::vector<int> v4;
	v4.push_back(4);
	v4.push_back(23);
	v4.push_back(18);
	v4.push_back(31);
	v4.push_back(167);
	v4.push_back(213);
	v4.push_back(86);
	std::vector<int> v5;
	v5.push_back(-7);
	v5.push_back(10);
	v5.push_back(806);
	v5.push_back(211);
	// create v6 by concatenating v4 and v5
	std::vector<int> v6(v4);
	for(uint i=0; i<v5.size(); ++i) { v6.push_back(v5[i]); }

	assert(v6.size() == 11);
	assert(v6[6] == 86);
	assert(v6[7] == -7);
	assert(v6[10] == 211);

	// compare some vectors
	assert(vector_compare(v1, v4));
	assert(! vector_compare(v5, v1));
	assert(vector_compare(v6, v3));
	assert(! vector_compare(v3, v6));
	assert(! vector_compare(v2, v5));
	assert(! vector_compare(v6, v1));

	// now concatenate everything into a big vector
	std::vector<int> all(v1);
	all.insert(all.end(), v2.begin(), v2.end());
	all.insert(all.end(), v3.begin(), v3.end());
	// v4 and v5 are already concatenated into v6
	// so just concatenate that
	all.insert(all.end(), v6.begin(), v6.end());
	assert(all.size() == 39);


	// count the numbers in it that are multiples of 3 and put them into a
	// new vector
	std::vector<int> threes;

	std::cout << "Now counting numbers divisible by 3" << std::endl;
	for(uint i = 0; i < all.size(); i+=1) {
		if(i % 3 == 0) {
			// std::cout << all[i] << " is divisible by 3" << std::endl;
			counter++;
			threes.push_back(i);
		}
	}
	std::cout << "There are " << counter << " numbers divisible by 3."
			<< std::endl;

	// loop over it and print it out backwards
	for(int i = counter-1; i >= 0; --i) {
		std::cout << "threes[" << i << "] = " << threes[i] << std::endl;
	}

	// DON'T TOUCH ANYTHING IN THIS FUNCTION AFTER THIS LINE
	std::cout << "Finished the vector operations" << std::endl;
	counter += v1sumsum + v2sumsum;
	counter += (all.size() == 40);
	for(uint i=13;i<24;++i) { counter += (int) all[i]; }
	return counter;
}


int triangle_operations() {
	Triangle t1(0,0, 3,6, 5, 2, "Myriel");
	Triangle t2(3,4, 7,4, 5,10, "Aginor"); // should be isosceles
	Triangle t3(-1,-1, 0,2, -4, -6, "Estermont");
	Triangle t4(60,61, 62,63, 59, 57, "Gamling");
	// this next one should be "equilateral"
	// in reality the sides are roughly 0.004 off, but it's good enough for
	// close_enough
	Triangle t5(-56,0, 0,97, 56,0, "Twoflower");
	Triangle t6(2,6, 2,-4, 4,1, "Marvin"); // should be isosceles
	Triangle t7(0,0, 0,4, 3,0, "Queequeg"); // 3-4-5
	Triangle t8(-2,-2, -2,13, 6,-2, "Dennys"); // 8-15-17

	// try testing types
	assert(t1.get_type() == "Scalene");
	assert(t2.get_type() == "Isosceles");
	assert(t4.get_type() == "Scalene");
	assert(t5.get_type() == "Equilateral");
	assert(t6.get_type() == "Isosceles");
	assert(t7.get_type() == "Scalene");
	assert(t8.get_type() == "Scalene");

	// print perimeters and areas
	std::cout << "Perimeter of t1: " << t1.get_perimeter() << std::endl;
	std::cout << "Perimeter of t2: " << t2.get_perimeter() << std::endl;
	std::cout << "Perimeter of t3: " << t3.get_perimeter() << std::endl;
	std::cout << "Perimeter of t4: " << t4.get_perimeter() << std::endl;
	std::cout << "Perimeter of t5: " << t5.get_perimeter() << std::endl;
	std::cout << "Perimeter of t6: " << t6.get_perimeter() << std::endl;
	std::cout << "Area of t1: " << t1.get_area() << std::endl;
	std::cout << "Area of t2: " << t2.get_area() << std::endl;
	std::cout << "Area of t3: " << t3.get_area() << std::endl;
	std::cout << "Area of t4: " << t4.get_area() << std::endl;
	std::cout << "Area of t5: " << t5.get_area() << std::endl;
	std::cout << "Area of t6: " << t6.get_area() << std::endl;

	// do some asserts for areas/perimeters of easy triangles
	// floating point math may give some results that are slightly off and
	// thus will fail a == test
	assert(close_enough(t2.get_area(), 12));
	assert(close_enough(t5.get_area(), 5432));
	assert(close_enough(t7.get_area(), 6));
	assert(close_enough(t7.get_perimeter(), 12));
	assert(close_enough(t8.get_area(), 60));
	assert(close_enough(t8.get_perimeter(), 40));

//*****************************************************************************
// The remainder of this function has NO bugs
	std::cout << "Finished the triangle operations" << std::endl;
	return (int) (t1.get_area() * (t7.get_type()[4] + (t8.get_perimeter() * t5.get_perimeter())) + t7.get_perimeter());
//*****************************************************************************
}
