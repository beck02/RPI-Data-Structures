#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_
#include <iostream>


// "prototype" of the Mesh class (defined in mesh.h) so we can store
// a pointer inside the Edge
class Point; 
class Vertex;
class Mesh;


// =================================================================

class Triangle {
public:

  // CONSTRUCTOR
  Triangle(Vertex* a, Vertex* b, Vertex* c, Mesh *m);

  // ACCESSORS
  int getID() const { return id; }
  Vertex* getVertex(int i) const;
  bool HasVertex (Vertex *pt) const;
  double getArea() const;
  double getAreaAfterReplacement(Vertex *v, const Point& p) const;
  bool RightSideUp() const;
  bool RightSideUpAfterReplacement(Vertex *v, const Point& p) const;
  
private:
  // REPRESENTATION
  int id;
  Vertex* pts[3];
  Mesh *mesh;
};


// =================================================================

// Related helper functions
double getArea(const Point& a, const Point& b, const Point& c);
double AngleBetween(const Point& a, const Point& b, const Point& c);
bool RightSideUp(const Point& a, const Point& b, const Point& c);


// Allows easy printing of triangles
std::ostream& operator<<(std::ostream& ostr, const Triangle& t);


// A more intuitive ordering function for Triangle pointers when they
// are stored in a set.  We will sort them by ID.  (Without this they
// would be sorted by pointer value/memory location, which is not
// intuitive and would change on subsequent program runs.)
class TriangleByID {
public:
  bool operator()(Triangle* a, Triangle *b) const {
    return (a->getID() < b->getID());
  }
};

// =================================================================

#endif
