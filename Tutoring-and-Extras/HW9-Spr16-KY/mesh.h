#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <cassert>
#include <map>
#include <set>

#include "mtrand.h"

#include "vertex.h"
#include "edge.h"
#include "triangle.h"

#include "priority_queue.h"


// the image will be scaled to a 1000x1000 bounding box
#define MAXIMUM_SVG 1000
// with a 20 pixel wide border
#define BORDER 20


// a few helper typedefs
// 
// NOTE: We use custom ordering functions for object pointers.
// Sorting by id rather than by memory address will ensure the
// ordering is consistent and repeatable.
//
typedef std::set<Vertex*,  VertexByID>   vertices_set;
typedef std::map<std::pair<Vertex*,Vertex*>,Edge*, VertexPairByID> edges_map;
typedef std::set<Triangle*, TriangleByID> triangles_set;


// =================================================================

class Mesh {
public:

	// CONSTRUCTOR & DESTRUCTOR
	Mesh(const std::string &filename, int num_rows_, int num_cols_,
			 const std::string &which, const std::string &method, 
			 bool preserve_area, bool debug);
	~Mesh();

	// ===================================================
	// ACCESSORS
	int numVertices() const { return vertices.size(); }
	int numEdges() const { return edges.size(); }
	int numTriangles() const { return triangles.size(); }
	bool PreserveArea() const { return preserve_area; }
	double getArea() const;
	// output
	void CreateSVG(const std::string& filename) const;
	// helper function for simplification
	bool isLegalCollapse(Edge *e) const;
	std::string getWhichCollapse() const { return which; }

	// ===================================================
	// MODIFIERS
	// simplification
	bool Collapse();
	void Simplify(int target_count);
	void CollectEdgesWithVertex(Vertex *a,std::set<Edge*>& r_e) const;
	void ReCalculateEdges(const std::set<Edge*>& r_e);

	// sanity check
	void Check() const;

private:
	// ===================================================
	// private helper functions for mesh manipulation
	void AddVertex(Vertex *a);
	void RemoveVertex(Vertex *a);
	void AddEdge(Vertex *a, Vertex *b);
	void RemoveEdge(Vertex *a, Vertex *b);
	void AddTriangle(Vertex *a, Vertex *b, Vertex *c);
	void RemoveTriangle(Triangle *t);

	// private helper functions for simplification
	Edge* FindEdge() const;

	// ===================================================
	// REPRESENTATION
	// dimensions
	double height;
	double width;

	// command line arguments
	std::string which;
	std::string method;
	bool preserve_area;
	bool debug;

	// containers to store the mesh elements
	vertices_set vertices;
	edges_map edges;
	triangles_set triangles;

	//
	// ASSIGNMENT: Edges will be additionally stored in a priority queue
	//   to efficiently identify the next edge to collapse
	
	PriorityQueue<Edge*> edges_pq;
	

	MTRand mtrand;

	// NOTE: We store the next random edge so that we can visualize it
	// before collapse. 
	Edge* next_random_edge;

};

// ====================================================================

// Allows easy printing of the overall element counts in the Mesh
std::ostream& operator<<(std::ostream &ostr, const Mesh& m);

// ====================================================================

#endif
