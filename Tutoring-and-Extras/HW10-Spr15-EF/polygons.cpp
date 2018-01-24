#include "polygons.h"

//======================================================================================
//======================================================================================

Polygon::Polygon(std::string n, std::vector<Point> v) : name(n), vertices(v){
	for(int i = 0; i < vertices.size(); i++){
		if(i == vertices.size()-1){
			sides.push_back(Vector(vertices[i], vertices[0]));
		} else{
			sides.push_back(Vector(vertices[i], vertices[i+1]));
		}
		if(i == 0){
			angles.push_back(Angle(vertices[vertices.size()-1], vertices[i], vertices[i+1]));
		} else if(i == vertices.size()-1){
			angles.push_back(Angle(vertices[i-1], vertices[i], vertices[0]));
		} else{
			angles.push_back(Angle(vertices[i-1], vertices[i], vertices[i+1]));
		}
	}
}

bool Polygon::HasAllEqualSides(){ 
	double len = sides[0].Length();
	for(int i = 0; i < sides.size(); i++){
		if(!EqualSides(sides[i].Length(), len)){
			return false;
		}
	}
	return true;
}

bool Polygon::HasAllEqualAngles(){
	double ang = angles[0];
	for(int i = 0; i < angles.size(); i++){
		if(!EqualAngles(angles[i], ang)){
			return false;
		}
	}
	return true;
}

bool Polygon::HasARightAngle(){
	for(int i = 0; i < angles.size(); i++){
		if(RightAngle(angles[i])){
			return true;
		}
	}
	return false;
}

bool Polygon::HasAnObtuseAngle(){
	for(int i = 0; i < angles.size(); i++){
		if(ObtuseAngle(angles[i])){
			return true;
		}
	}
	return false;
}

bool Polygon::HasAnAcuteAngle(){
	for(int i = 0; i < angles.size(); i++){
		if(AcuteAngle(angles[i])){
			return true;
		}
	}
	return false;
}

bool Polygon::IsConvex(){
	for(int i = 0; i < angles.size(); i++){
		if(ReflexAngle(angles[i])){
			return false;
		}
	}
	return true;
}

bool Polygon::IsConcave(){
	for(int i = 0; i < angles.size(); i++){
		if(ReflexAngle(angles[i])){
			return true;
		}
	}
	return false;
}

//======================================================================================
//======================================================================================

Triangle::Triangle(std::string n, std::vector<Point> v) : Polygon(n, v) {

}

//================================================================

Quadrilateral::Quadrilateral(std::string n, std::vector<Point> v) : Polygon(n, v) {

}

//======================================================================================
//===============------INTERMEDIATE TRIANGLES------=====================================
//======================================================================================

RightTriangle::RightTriangle(std::string n, std::vector<Point> v) : Triangle(n, v) {

};

//================================================================

IsoscelesTriangle::IsoscelesTriangle(std::string n, std::vector<Point> v) : Triangle(n, v) {

}

//================================================================

ObtuseTriangle::ObtuseTriangle(std::string n, std::vector<Point> v) : Triangle(n, v) {

}

//================================================================

IsoscelesRightTriangle::IsoscelesRightTriangle(std::string n, std::vector<Point> v) : Triangle(n, v), IsoscelesTriangle(n, v), RightTriangle(n, v) {

}

//================================================================

EquilateralTriangle::EquilateralTriangle(std::string n, std::vector<Point> v) : Triangle(n, v), IsoscelesTriangle(n, v) {

}

//================================================================

IsoscelesObtuseTriangle::IsoscelesObtuseTriangle(std::string n, std::vector<Point> v) : Triangle(n, v), IsoscelesTriangle(n, v), ObtuseTriangle(n, v) {

}

//======================================================================================
//=================------INTERMEDIATE QUADS------=======================================
//======================================================================================

Kite::Kite(std::string n, std::vector<Point> v) : Quadrilateral(n, v) {

}

//================================================================

Arrow::Arrow(std::string n, std::vector<Point> v) : Quadrilateral(n, v) {

}

//================================================================

Trapezoid::Trapezoid(std::string n, std::vector<Point> v) : Quadrilateral(n, v) {

}

//================================================================

Parallelogram::Parallelogram(std::string n, std::vector<Point> v) : Quadrilateral(n, v), Trapezoid(n, v) {

}

//================================================================

IsoscelesTrapezoid::IsoscelesTrapezoid(std::string n, std::vector<Point> v) : Quadrilateral(n, v), Trapezoid(n, v){

}

//======================================================================================
//====================------BASIC SHAPES------==========================================
//======================================================================================

Rhombus::Rhombus(std::string n, std::vector<Point> v) : Quadrilateral(n, v), Trapezoid(n, v), Parallelogram(n, v), Kite(n, v) {

}

//================================================================

Rectangle::Rectangle(std::string n, std::vector<Point> v) : Quadrilateral(n, v), Trapezoid(n, v), Parallelogram(n, v), IsoscelesTrapezoid(n, v) {

}

//================================================================

Square::Square(std::string n, std::vector<Point> v) : Quadrilateral(n, v), Trapezoid(n, v), Parallelogram(n, v), Rhombus(n, v), Rectangle(n, v) {

}

//======================================================================================
//======================================================================================







