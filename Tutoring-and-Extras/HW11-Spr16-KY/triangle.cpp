#include <cassert>
#include "triangle.h"

//*****************************************************************************
// There ARE errors in this file
//*****************************************************************************
// This file defines the functions in our Triangle class
// Triangle objects exist in the X-Y coordinate plane
//*****************************************************************************
// Note that when comparing floats here, in most cases we will be using
// close_enough(), found in operations.cpp.
//*****************************************************************************


//*****************************************************************************
// Calculate the area of this triangle, using Heron's Formula
// Details found here: https://en.wikipedia.org/wiki/Heron%27s_formula
// Formula:
//   Given any triangle with side lengths a, b, c
//   Let s = (a+b+c)/2
//   Then area = sqrt[ s * (s-a) * (s-b) * (s-c) ]
float Triangle::get_area() const {
  double a = length_between(x1, x2, y1, y2);
  double b = length_between(x2, x3, y2, y3);
  double c = length_between(x1, x3, y1, y3);
  double s = (a+b+c)/2;

  return sqrt(s*(s-a)*(s-b)*(s-c));
}

//*****************************************************************************
// Calculate the perimeter of this triangle
// Formula:
//   Given a triangle with side lengths a, b, c
//   Then perimeter = a + c + c
float Triangle::get_perimeter() const {
  return length_between(x1, x2, y1, y2) +
    length_between(x1, x3, y1, y3) +
    length_between(x2, x3, y2, y3);
}

//*****************************************************************************
// Calculate the "type" of this triangle
// Valid types are: "Equilateral", "Isosceles", "Scalene"
// Definitions:
//   An Equilateral triangle has three equal length sides
//   An Isosceles triangle has only two sides with equal length
//   A Scalene triangle has no sides with equal lengths
std::string Triangle::get_type() const {
  double a = length_between(x1, x2, y1, y2);
  double b = length_between(x2, x3, y2, y3);
  double c = length_between(x1, x3, y1, y3);
  if(close_enough(a,b) && close_enough(b,c)) {
    return "Equilateral";
  }
  else if(close_enough(a,c) || close_enough(a,b) || close_enough(b,c)) {
    return "Isosceles";
  }
  else {
    return "Scalene";
  }
}

//*****************************************************************************
// Print the details of this triangle
// This function contains NO bugs
void Triangle::print() const {
  std::cout << "[Name: " << name << "] Points: (" << x1 << "," << y1 << "), ("
	    << x2 << "," << y2 << "), (" << x3 << "," << y3 << ")"
	    << std::endl;
}

//*****************************************************************************
// HELPER FUNCTION
// Calculate the Euclidean distance between two points.
// Formula: Distance between (x1,y1) and (x2,y2) = sqrt[ (x2-x1)^2 + (y2-y1)^2 ]
double length_between(int x1, int x2, int y1, int y2) {
  return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

//*****************************************************************************
// HELPER FUNCTION
// This function should test *all* the functionality of each triangle function
// YOU MUST CREATE SOME ADDITIONAL TESTS
void triangle_bug_tester() {

  // Test the constructor function
  Triangle t1(1,2,3,4,5,6, "TestPrivates");
  assert(t1.get_x1() == 1);
  assert(t1.get_y1() == 2);
  assert(t1.get_x2() == 3);
  assert(t1.get_y2() == 4);
  assert(t1.get_x3() == 5);
  assert(t1.get_y3() == 6);
  assert(t1.get_name() == "TestPrivates");

  // Test Triangle::get_area

  // Test Triangle::get_perimeter

  // Test Triangle::get_type

  // Test Triangle::print

  // Test close_enough
  double d1 = 0.01;
  double d2 = 0.0;
  double d3 = 0.009;
  double d4 = 500000000000.000001;
  double d5 = 500000000000.00001;
  assert (close_enough(d1, d2) == 0);
  assert (close_enough(d2, d3) == 1);
  assert (close_enough(d4, d5) == 1);

  // Test length_between
  assert(length_between(0, 0, 0, 1) == 1);
  assert(length_between(0, 0, 1, 0) == 1);
  assert(length_between(0, 0, 1, 1) == sqrt(2.0));
  assert(close_enough(length_between(0, 0, 1, 1), sqrt(2.0)) == 1);

}
