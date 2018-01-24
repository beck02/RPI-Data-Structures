#ifndef _POLYS_H_
#define _POLYS_H_

#include <vector>
#include <string>
#include "utilities.h"

//======================================================================================
//====================------POLYGON------===============================================
//======================================================================================

class Polygon{
public:
	Polygon(const std::string& n, const std::vector<Point>& v);// : name(n), vertices(v) {}
	virtual ~Polygon() {}
	const std::string& getName() const { return name; }
	//info methods
	virtual bool HasAllEqualSides() const;
	virtual bool HasAllEqualAngles() const;
	virtual bool HasARightAngle() const;
	virtual bool HasAnObtuseAngle() const;
	virtual bool HasAnAcuteAngle() const;
	virtual bool IsConvex() const;
	virtual bool IsConcave() const;

protected:
	std::string name;
	std::vector<Point> vertices;
	std::vector<Vector> sides;
	std::vector<double> angles;
};

//======================================================================================
//====================------BASIC SHAPES------==========================================
//======================================================================================

class Triangle : public Polygon {
public:
	Triangle(const std::string& n, const std::vector<Point>& v);// : Polygon(n, v) {}
	bool HasAnAcuteAngle() const { return true; }
	bool IsConcave() const { return false; }
	bool IsConvex() const { return true; }
};

//================================================================

class Quadrilateral : public Polygon {
public: 
	Quadrilateral(const std::string& n, const std::vector<Point>& v);// : Polygon(n, v) {}
};

//======================================================================================
//===============------INTERMEDIATE TRIANGLES------=====================================
//======================================================================================

class RightTriangle : public virtual Triangle{
public:
	RightTriangle(const std::string& n, const std::vector<Point>& v);// : Triangle(n, v) {}
	bool HasAnObtuseAngle() const { return false; }
	bool HasARightAngle() const { return true; }
	bool HasAllEqualAngles() const { return false; }
	bool HasAllEqualSides() const { return false; }
};

//================================================================

class IsoscelesTriangle : public virtual Triangle {
public:
	IsoscelesTriangle(const std::string& n, const std::vector<Point>& v);
};

//================================================================

class ObtuseTriangle : public virtual Triangle{
public:
	ObtuseTriangle(const std::string& n, const std::vector<Point>& v);
	bool HasAnObtuseAngle() const { return true; }
	bool HasARightAngle() const { return false; }
	bool HasAllEqualAngles() const { return false; }
	bool HasAllEqualSides() const { return false; }
};

//======================================================================================
//==================------BOTTOM TRIANGLES------========================================
//======================================================================================

class IsoscelesRightTriangle : public IsoscelesTriangle, public RightTriangle{
public:
	IsoscelesRightTriangle(const std::string& n, const std::vector<Point>& v);
};

//================================================================

class EquilateralTriangle : public IsoscelesTriangle {
public:
	EquilateralTriangle(const std::string& n, const std::vector<Point>& v);
	bool HasAllEqualSides() const { return true; }
	bool HasAllEqualAngles() const { return true; }
	bool HasAnObtuseAngle() const { return false; }
	bool HasARightAngle() const { return false; }
};

//================================================================

class IsoscelesObtuseTriangle : public IsoscelesTriangle, public ObtuseTriangle{
public:
	IsoscelesObtuseTriangle(const std::string& n, const std::vector<Point>& v);
};

//======================================================================================
//=================------INTERMEDIATE QUADS------=======================================
//======================================================================================

class Kite : public virtual Quadrilateral{
public:
	Kite(const std::string& n, const std::vector<Point>& v);
	bool IsConvex() const { return true; }
	bool IsConcave() const { return false; }
};

//================================================================

class Arrow : public Quadrilateral{
public:
	Arrow(const std::string& n, const std::vector<Point>& v);
	bool IsConvex() const { return false; }
	bool IsConcave() const { return true; }
	bool HasAnAcuteAngle() const { return true; }
};

//================================================================

class Trapezoid : public virtual Quadrilateral{
public:
	Trapezoid(const std::string& n, const std::vector<Point>& v);
	bool IsConvex() const { return true; }
	bool IsConcave() const { return false; }
};

//================================================================

class Parallelogram : public virtual Trapezoid{
public:
	Parallelogram(const std::string& n, const std::vector<Point>& v);
};

//================================================================

class IsoscelesTrapezoid : public virtual Trapezoid{
public:
	IsoscelesTrapezoid(const std::string& n, const std::vector<Point>& v);
};

//======================================================================================
//====================------BOTTOM QUADS------==========================================
//======================================================================================

class Rhombus : public Kite, public virtual Parallelogram {
public: 
	Rhombus(const std::string& n, const std::vector<Point>& v);
	bool HasAllEqualSides() const { return true; }
	bool IsConvex() const { return false; }
	bool IsConcave() const { return true; }
};

//================================================================

class Rectangle : public IsoscelesTrapezoid, public virtual Parallelogram{
public: 
	Rectangle(const std::string& n, const std::vector<Point>& v);
	bool HasAllEqualAngles() const { return true; }
	bool HasARightAngle() const { return true; }
	bool HasAnAcuteAngle() const { return false; }
	bool HasAnObtuseAngle() const { return false; }
};

//================================================================

class Square : public Rhombus, public Rectangle{
public:
	Square(const std::string& n, const std::vector<Point>& v);
};

//======================================================================================
//======================================================================================

#endif




