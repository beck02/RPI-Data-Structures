#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cmath>
#include <string>
#include <iostream>

//*****************************************************************************
// There ARE errors in this file
//*****************************************************************************
// This file declares our Triangle class
// Triangle objects exist in the X-Y coordinate plane
// The functions declared here are defined in triangle.cpp
//*****************************************************************************

class Triangle {
  // internal representation
private:
  int x1, y1, x2, y2, x3, y3;
  std::string name;

public:
  // Constructor Functions
  Triangle(int x1_, int y1_, int x2_, int y2_, int x3_, int y3_,
	   std::string name_) :
    x1(x1_), y1(y1_), x2(x2_), y2(y2_), x3(x3_), y3(y3_), name(name_) {}

  // Accessor Functions
  float get_area() const;
  float get_perimeter() const;
  std::string get_type() const;
  void print() const;

  int get_x1() const { return x1; }
  int get_y1() const { return y1; }
  int get_x2() const { return x2; }
  int get_y2() const { return y2; }
  int get_x3() const { return x3; }
  int get_y3() const { return y3; }
  std::string get_name() const { return name; }
};

// HELPER FUNCTIONS
// non-member functions that relate to triangles
bool close_enough(double d1, double d2);
double length_between(int x1, int y1, int x2, int y2);
void triangle_bug_tester();

#endif
