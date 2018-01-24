#include <iomanip>

#include "vertex.h"
#include "edge.h"
#include "triangle.h"

// ====================================================================
// Point functions

Point Average(const Point &a, const Point &b) {
  double x = (a.x + b.x) / 2.0;
  double y = (a.y + b.y) / 2.0;
  return Point(x,y);
}

double DistanceBetween(const Point &a, const Point &b) {
  return sqrt((a.x-b.x)*(a.x-b.x) +
              (a.y-b.y)*(a.y-b.y));
}

// ====================================================================
// Color functions

Color Average(const Color &a, const Color &b) {
  float r_ = (a.r + b.r) / 2.0;
  float g_ = (a.g + b.g) / 2.0;
  float b_ = (a.b + b.b) / 2.0;
  return Color(r_,g_,b_);
}


// ====================================================================
// Vertex functions

Vertex::Vertex(double x_, double y_,
       float r_, float g_, float b_) : pt(x_,y_), col(r_,g_,b_) {
  // each vertex gets a unique ID
  static int counter = 0;
  id = counter;
  counter++;
}


void Vertex::Average(Vertex *v) {
  pt = ::Average(pt,v->pt);
  col = ::Average(col,v->col);
}


// functions to maintain the neighborhood of a vertex
// each Vertex stores the triangles and edges it touches

void Vertex::AddTriangle(Triangle* t) {
  assert (t->HasVertex(this));
  std::pair<std::set<Triangle*>::iterator,bool> ret = triangles.insert(t);
  assert (ret.second == true);
}

void Vertex::AddEdge(Edge* e){ 
  assert (e->getV1() == this || e->getV2() == this);
  std::pair<std::set<Edge*>::iterator,bool> ret = edges.insert(e);
  assert (ret.second == true);
}

void Vertex::RemoveTriangle(Triangle* t) {
  assert (t->HasVertex(this));
  int ret = triangles.erase(t);
  assert (ret == 1);
}

void Vertex::RemoveEdge(Edge* e) {
assert (e->getV1() == this || e->getV2() == this);
  int ret = edges.erase(e);
  assert (ret == 1);  
}


// stream printing function for Vertex
std::ostream& operator<<(std::ostream& ostr, const Vertex& v) {
  ostr << " VERTEX " 
       << std::setw(6) << v.getID() << " <" 
       << std::setw(8) << std::fixed << std::setprecision(3) << v.x() 
       << "," 
       << std::setw(8) << std::fixed << std::setprecision(3) << v.y() 
       << ">";
  return ostr;
}

// ====================================================================
