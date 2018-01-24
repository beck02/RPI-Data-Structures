#ifndef _POINT_H_
#define _POINT_H_

#include <cmath>
#include <cassert>
#include <iostream>
#include <set>


// ====================================================================
// Point: Helper class for the geometry of a Vertex

class Point {
public:
  Point(double x_, double y_) : x(x_),y(y_) {}
  double x;
  double y;
};

Point Average(const Point &a, const Point &b);
double DistanceBetween(const Point &a, const Point &b);

// ====================================================================
// Color: Helper class for the r,g,b values of a Vertex

class Color {
public:
  Color(int r_=255, int g_=255, int b_=255) : r(r_),g(g_),b(b_) {}
  bool isWhite() const { return r==255 && g==255 && b==255; }
  bool isBlack() const { return r==0 && g==0 && b==0; }
  int r,g,b;
};

Color Average(const Color &a, const Color &b);


// ====================================================================


// "prototype" of the Edge & Triangle classs (defined in edge.h and
// triangle.h) so we can store pointers to them inside Edge
class Edge;
class Triangle;


// ====================================================================

class Vertex {
public:

  // CONSTRUCTOR
  Vertex(double x_, double y_,float r_, float g_, float b_);
  
  // ACCESSORS
  const Point& getPoint() const { return pt; }
  double x() const { return pt.x; }
  double y() const { return pt.y; }
  float r() const { return col.r; }
  float g() const { return col.g; }
  float b() const { return col.b; }
  int getID() const { return id; }
  const std::set<Edge*>& getEdges() const { return edges; }
  const std::set<Triangle*>& getTriangles() const { return triangles; }

  // MODIFIERS
  void Average(Vertex *v);
  void AddTriangle(Triangle* t);
  void AddEdge(Edge* e);
  void RemoveTriangle(Triangle* t);
  void RemoveEdge(Edge* e);

private:
  // REPRESENTATION
  int id;
  Point pt;
  Color col;
  // for efficient lookup, store pointers to the elements that use
  // this vertex
  std::set<Triangle*> triangles;
  std::set<Edge*> edges;
};

// ====================================================================


// Allows easy printing of vertices
std::ostream& operator<<(std::ostream& ostr, const Vertex& v);


// A more intuitive ordering function for Vertex pointers when they
// are stored in a set.  We will sort them by ID.  (Without this they
// would be sorted by pointer value/memory location, which is not
// intuitive and would change on subsequent program runs.)
class VertexByID {
public:
  bool operator()(Vertex* a, Vertex *b) const {
    return (a->getID() < b->getID());
  }
};

// ====================================================================


#endif
