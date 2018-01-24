#ifndef _EDGE_H_
#define _EDGE_H_
#include <iostream>

#include "vertex.h"


// "prototype" of the Mesh class (defined in mesh.h) so we can store
// a pointer inside the Edge
class Mesh;


// =================================================================

class Edge {

public:

  // CONSTRUCTOR
  Edge(Vertex *a, Vertex *b, Mesh *m);
  
  // ACCESSORS
  Vertex* getV1() const { return v1; } 
  Vertex* getV2() const { return v2; } 
  bool isLegal() const { return legal; }
  double getLength() const { return length; }
  double getPriorityValue() const;

  bool CheckValue() const;

  // MODIFIER
  void ReCalculateValue();

private:
  // REPRESENTATION
  Vertex *v1;
  Vertex *v2;
  bool legal;
  double length;
  Mesh *mesh;
};

// =================================================================

// these operations are necessary so we can put Edges in a PriorityQueue
bool operator==(const Edge &a, const Edge& b);
bool operator<(const Edge &a, const Edge& b);


// Allows easy printing of edges & edge pointers
std::ostream& operator<<(std::ostream &ostr, const Edge& e);
std::ostream& operator<<(std::ostream &ostr, Edge* e);


// A more intuitive ordering function for a pair of Vertex pointers,
// so we can store this thing in a map.  We will sort them by ID.
// (Without this they would be sorted by pointer value/memory
// location, which is not intuitive and would change on subsequent
// program runs.)
class VertexPairByID {
public:
  bool operator()(std::pair<Vertex*,Vertex*> a,
                  std::pair<Vertex*,Vertex*> b) const {

    assert (a.first->getID() < a.second->getID());
    assert (b.first->getID() < b.second->getID());

    return (a.first->getID() < b.first->getID() ||
            (a.first->getID() == b.first->getID() &&
             a.second->getID() < b.second->getID()));

  }
};

// =================================================================

#endif
