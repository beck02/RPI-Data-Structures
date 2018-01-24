#ifndef _POLYS_H_
#define _POLYS_H_

#include <vector>
#include <string>
#include "utilities.h"

//======================================================================================
//======================================================================================

class Polygon{
public:
	Polygon(std::string n, std::vector<Point> v);// : name(n), vertices(v) {}
	std::string getName() const { return name; }
	//info methods
	virtual bool HasAllEqualSides();
	virtual bool HasAllEqualAngles();
	virtual bool HasARightAngle();
	virtual bool HasAnObtuseAngle();
	virtual bool HasAnAcuteAngle();
	virtual bool IsConvex();
	virtual bool IsConcave();

protected:
	std::string name;
	std::vector<Point> vertices;
	std::vector<Vector> sides;
	std::vector<double> angles;
};

//================================================================

class Triangle : public Polygon {
public:
	Triangle(std::string n, std::vector<Point> v);// : Polygon(n, v) {}
	bool HasAnAcuteAngle() { return true; }
};

//================================================================

class Quadrilateral : public Polygon {
public: 
	Quadrilateral(std::string n, std::vector<Point> v);// : Polygon(n, v) {}
};

//======================================================================================
//===============------INTERMEDIATE TRIANGLES------=====================================
//======================================================================================

class RightTriangle : public virtual Triangle{
public:
	RightTriangle(std::string n, std::vector<Point> v);// : Triangle(n, v) {}
	bool HasARightAngle() { return true; }
	bool HasAnObtuseAngle() { return false; }
};

//================================================================

class IsoscelesTriangle : public virtual Triangle {
public:
	IsoscelesTriangle(std::string n, std::vector<Point> v);
};

//================================================================

class ObtuseTriangle : public virtual Triangle{
public:
	ObtuseTriangle(std::string n, std::vector<Point> v);
	bool HasAnObtuseAngle() { return true; }
	bool HasARightAngle() { return false; }
};

//================================================================

class IsoscelesRightTriangle : public IsoscelesTriangle, public RightTriangle{
public:
	IsoscelesRightTriangle(std::string n, std::vector<Point> v);
};

//================================================================

class EquilateralTriangle : public IsoscelesTriangle {
public:
	EquilateralTriangle(std::string n, std::vector<Point> v);
	bool HasAllEqualSides() { return true; }
	bool HasAllEqualAngles() { return true; }
	bool HasARightAngle() { return false; }
	bool HasAnObtuseAngle() { return false; }
	bool HasAnAcuteAngle() { return true; }
};

//================================================================

class IsoscelesObtuseTriangle : public IsoscelesTriangle, public ObtuseTriangle{
public:
	IsoscelesObtuseTriangle(std::string n, std::vector<Point> v);
};

//======================================================================================
//=================------INTERMEDIATE QUADS------=======================================
//======================================================================================

class Kite : public virtual Quadrilateral{
public:
	Kite(std::string n, std::vector<Point> v);
};

//================================================================

class Arrow : public Quadrilateral{
public:
	Arrow(std::string n, std::vector<Point> v);
	bool HasAnObtuseAngle() { return true; }
	bool HasAnAcuteAngle() { return true; }
	bool IsConcave() { return true; }
	bool IsConvex() { return false; }
};

//================================================================

class Trapezoid : public virtual Quadrilateral{
public:
	Trapezoid(std::string n, std::vector<Point> v);
};

//================================================================

class Parallelogram : public virtual Trapezoid{
public:
	Parallelogram(std::string n, std::vector<Point> v);
};

//================================================================

class IsoscelesTrapezoid : public virtual Trapezoid{
public:
	IsoscelesTrapezoid(std::string n, std::vector<Point> v);
};

//======================================================================================
//====================------BASIC SHAPES------==========================================
//======================================================================================

class Rhombus : public Kite, public virtual Parallelogram {
public: 
	Rhombus(std::string n, std::vector<Point> v);
	bool HasAllEqualSides() { return true; }
};

//================================================================

class Rectangle : public IsoscelesTrapezoid, public virtual Parallelogram{
public: 
	Rectangle(std::string n, std::vector<Point> v);
	bool HasARightAngle() { return true; }
	bool HasAnObtuseAngle() { return false; }
	bool HasAnAcuteAngle() { return false; }
	bool HasAllEqualAngles() { return true; }
};

//================================================================

class Square : public Rhombus, public Rectangle{
public:
	Square(std::string n, std::vector<Point> v);
};

//======================================================================================
//======================================================================================

#endif








