#include "polygons.h"

//======================================================================================
//====================------POLYGON------===============================================
//======================================================================================

Polygon::Polygon(const std::string& n, const std::vector<Point>& v) : name(n), vertices(v){
	if(v.size() < 3){ throw -1; }
	for(int i = 0; i < vertices.size(); i++){
		if(i == 0){
			sides.push_back(Vector(vertices[i], vertices[i+1]));
			angles.push_back(Angle(vertices[vertices.size()-1], vertices[i], vertices[i+1]));
		} else if(i == vertices.size()-1){
			sides.push_back(Vector(vertices[i], vertices[0]));
			angles.push_back(Angle(vertices[i-1], vertices[i], vertices[0]));
		} else{
			sides.push_back(Vector(vertices[i], vertices[i+1]));
			angles.push_back(Angle(vertices[i-1], vertices[i], vertices[i+1]));
		}
	}
}

//================================================================
//================================================================

bool Polygon::HasAllEqualSides() const{ 
	double len = sides[0].Length();
	for(int i = 0; i < sides.size(); i++){
		if(!EqualSides(sides[i].Length(), len)){
			return false;
		}
	}
	return true;
}

//================================================================

bool Polygon::HasAllEqualAngles() const{ 
	double ang = angles[0];
	for(int i = 0; i < angles.size(); i++){
		if(!EqualAngles(angles[i], ang)){
			return false;
		}
	}
	return true;
}

//================================================================

bool Polygon::HasARightAngle() const{
	for(int i = 0; i < angles.size(); i++){
		if(RightAngle(angles[i])){
			return true;
		}
	}
	return false;
}

//================================================================

bool Polygon::HasAnObtuseAngle() const{
	for(int i = 0; i < angles.size(); i++){
		if(ObtuseAngle(angles[i])){
			return true;
		}
	}
	return false;
}

//================================================================

bool Polygon::HasAnAcuteAngle() const{
	for(int i = 0; i < angles.size(); i++){
		if(AcuteAngle(angles[i])){
			return true;
		}
	}
	return false;
}

//================================================================

bool Polygon::IsConvex() const{
	for(int i = 0; i < angles.size(); i++){
		if(ReflexAngle(angles[i])){
			return false;
		}
	}
	return true;
}

//================================================================

bool Polygon::IsConcave() const{
	return !IsConvex();
}

//======================================================================================
//====================------BASIC SHAPES------==========================================
//======================================================================================

Triangle::Triangle(const std::string& n, const std::vector<Point>& v) : Polygon(n, v) {
	if(v.size() != 3){ throw -1; }
	if(Polygon::IsConvex() == false){ throw -1; }
	if(Polygon::HasAnAcuteAngle() == false){ throw -1; }
	double angleSum = angles[0] + angles[1] + angles[2];
	if(EqualAngles(angleSum, 180.0) == false){ throw -1; }
}

//================================================================

Quadrilateral::Quadrilateral(const std::string& n, const std::vector<Point>& v) : Polygon(n, v) {
	if(v.size() != 4){ throw -1; }
	double angleSum = angles[0] + angles[1] + angles[2] + angles[3];
	if(EqualAngles(angleSum/2.0, 180.0) == false){ throw -1; }
}

//======================================================================================
//===============------INTERMEDIATE TRIANGLES------=====================================
//======================================================================================

RightTriangle::RightTriangle(const std::string& n, const std::vector<Point>& v) : Triangle(n, v) {
	if(Polygon::HasAnObtuseAngle() == true){ throw -1; }
	if(Polygon::HasARightAngle() == false){ throw -1; }
	if(Polygon::HasAllEqualSides() == true){ throw -1; }
	if(Polygon::HasAllEqualAngles() == true){ throw -1; }
};

//================================================================

IsoscelesTriangle::IsoscelesTriangle(const std::string& n, const std::vector<Point>& v) : Triangle(n, v) {
	bool pair1 = EqualSides(sides[0].Length(), sides[1].Length()) && EqualAngles(angles[0], angles[2]);
	bool pair2 = EqualSides(sides[1].Length(), sides[2].Length()) && EqualAngles(angles[1], angles[0]);
	bool pair3 = EqualSides(sides[2].Length(), sides[0].Length()) && EqualAngles(angles[2], angles[1]);

	if(pair1 == false && pair2 == false && pair3 == false){ throw -1; }
}

//================================================================

ObtuseTriangle::ObtuseTriangle(const std::string& n, const std::vector<Point>& v) : Triangle(n, v) {
	if(Polygon::HasAnObtuseAngle() == false){ throw -1; }
	if(Polygon::HasARightAngle() == true){ throw -1; }
	if(Polygon::HasAllEqualSides() == true){ throw -1; }
	if(Polygon::HasAllEqualAngles() == true){ throw -1; }
}

//======================================================================================
//==================------BOTTOM TRIANGLES------========================================
//======================================================================================

IsoscelesRightTriangle::IsoscelesRightTriangle(const std::string& n, const std::vector<Point>& v) : Triangle(n, v), IsoscelesTriangle(n, v), RightTriangle(n, v) {

}

//================================================================

EquilateralTriangle::EquilateralTriangle(const std::string& n, const std::vector<Point>& v) : Triangle(n, v), IsoscelesTriangle(n, v) {
	if(Polygon::HasAllEqualSides() == false){ throw -1; }
	if(Polygon::HasAllEqualAngles() == false){ throw -1; }
	if(Polygon::HasAnObtuseAngle() == true){ throw -1; }
	if(Polygon::HasARightAngle() == true){ throw -1; }
}

//================================================================

IsoscelesObtuseTriangle::IsoscelesObtuseTriangle(const std::string& n, const std::vector<Point>& v) : Triangle(n, v), IsoscelesTriangle(n, v), ObtuseTriangle(n, v) {

}

//======================================================================================
//=================------INTERMEDIATE QUADS------=======================================
//======================================================================================

Kite::Kite(const std::string& n, const std::vector<Point>& v) : Quadrilateral(n, v) {
	if(Polygon::IsConvex() == false){ throw -1; }

	bool group1 = EqualSides(sides[0].Length(), sides[1].Length()) && EqualSides(sides[2].Length(), sides[3].Length());
	bool group2 = EqualSides(sides[1].Length(), sides[2].Length()) && EqualSides(sides[3].Length(), sides[0].Length());

	if(group1 == false && group2 == false){ throw -1; }
}

//================================================================

Arrow::Arrow(const std::string& n, const std::vector<Point>& v) : Quadrilateral(n, v) {
	if(Polygon::IsConvex() == true){ throw -1; }
	if(Polygon::HasAnAcuteAngle() == false){ throw -1; }

	bool group1 = EqualSides(sides[0].Length(), sides[1].Length()) && EqualSides(sides[2].Length(), sides[3].Length());
	bool group2 = EqualSides(sides[1].Length(), sides[2].Length()) && EqualSides(sides[3].Length(), sides[0].Length());

	if(group1 == false && group2 == false){ throw -1; }
}

//================================================================

Trapezoid::Trapezoid(const std::string& n, const std::vector<Point>& v) : Quadrilateral(n, v) {
	if(Polygon::IsConvex() == false){ throw -1; }
	if(!Parallel(sides[0], sides[2]) && !Parallel(sides[1], sides[3])){ throw -1; }
}

//================================================================

Parallelogram::Parallelogram(const std::string& n, const std::vector<Point>& v) : Quadrilateral(n, v), Trapezoid(n, v) {
	bool pair1 = Parallel(sides[0], sides[2]) && EqualSides(sides[0].Length(), sides[2].Length());
	bool pair2 = Parallel(sides[1], sides[3]) && EqualSides(sides[1].Length(), sides[3].Length());

	if(pair1 == false || pair2 == false){ throw -1; }

	if(!EqualAngles(angles[0], angles[2]) || !EqualAngles(angles[1], angles[3])){ throw -1; }
}

//================================================================

IsoscelesTrapezoid::IsoscelesTrapezoid(const std::string& n, const std::vector<Point>& v) : Quadrilateral(n, v), Trapezoid(n, v){
	double oppAngleSum1 = angles[0] + angles[2];
	double oppAngleSum2 = angles[1] + angles[3];

	if(EqualAngles(oppAngleSum1, 180.0) == false || EqualAngles(oppAngleSum2, 180.0) == false){ throw -1; }
}

//======================================================================================
//====================------BOTTOM QUADS------==========================================
//======================================================================================

Rhombus::Rhombus(const std::string& n, const std::vector<Point>& v) : Quadrilateral(n, v), Trapezoid(n, v), Parallelogram(n, v), Kite(n, v) {
	if(Polygon::HasAllEqualSides() == false){ throw -1; }
}

//================================================================

Rectangle::Rectangle(const std::string& n, const std::vector<Point>& v) : Quadrilateral(n, v), Trapezoid(n, v), Parallelogram(n, v), IsoscelesTrapezoid(n, v) {
	if(Polygon::HasAllEqualAngles() == false){ throw -1; }
	if(Polygon::HasAnObtuseAngle() == true){ throw -1; }
	if(Polygon::HasAnAcuteAngle() == true){ throw -1; }
	if(Polygon::HasARightAngle() == false){ throw -1; }
}

//================================================================

Square::Square(const std::string& n, const std::vector<Point>& v) : Quadrilateral(n, v), Trapezoid(n, v), Parallelogram(n, v), Rhombus(n, v), Rectangle(n, v) {

}

//======================================================================================
//======================================================================================













