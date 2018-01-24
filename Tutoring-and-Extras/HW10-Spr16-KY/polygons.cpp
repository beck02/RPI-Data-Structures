#include "polygons.h"

//======================================================================================
//====================------POLYGON------===============================================
//======================================================================================

Polygon::Polygon(const std::string& n, const std::vector<Point>& v) : name(n), vertices(v){
	if(v.size() < 3){ throw -1; }
}

//================================================================
//================================================================

bool Polygon::HasAllEqualSides() const{
	int last = vertices.size() - 1;
	double sideLength = DistanceBetween(vertices[last], vertices[0]);
	for(int i = 0; i < last; ++i){
		double dist = DistanceBetween(vertices[i], vertices[i+1]);
		if(EqualSides(sideLength, dist) == false){
			return false;
		}
	}
	return true;
}

//================================================================

bool Polygon::HasAllEqualAngles() const{ 
	int last = vertices.size() - 1;
	double angle = Angle(vertices[last], vertices[0], vertices[1]);
	for(int i = 1; i < vertices.size(); ++i){
		double degrees;
		if(i == last){
			degrees = Angle(vertices[i-1], vertices[i], vertices[0]);
		} else{
			degrees = Angle(vertices[i-1], vertices[i], vertices[i+1]);
		}
		if(EqualAngles(angle, degrees) == false){
			return false;
		}
	}
	return true;
}

//================================================================

bool Polygon::HasARightAngle() const{
	int last = vertices.size() - 1;
	for(int i = 0; i < vertices.size(); ++i){
		double angle;
		if(i == last){
			angle = Angle(vertices[i-1], vertices[i], vertices[0]);
		} else if(i == 0){
			angle = Angle(vertices[last], vertices[0], vertices[1]);
		} else{
			angle = Angle(vertices[i-1], vertices[i], vertices[i+1]);
		}
		if(RightAngle(angle) == true){
			return true;
		}
	}
	return false;
}

//================================================================

bool Polygon::HasAnObtuseAngle() const{
	int last = vertices.size() - 1;
	for(int i = 0; i < vertices.size(); ++i){
		double angle;
		if(i == last){
			angle = Angle(vertices[i-1], vertices[i], vertices[0]);
		} else if(i == 0){
			angle = Angle(vertices[last], vertices[0], vertices[1]);
		} else{
			angle = Angle(vertices[i-1], vertices[i], vertices[i+1]);
		}
		if(ObtuseAngle(angle) == true){
			return true;
		}
	}
	return false;
}

//================================================================

bool Polygon::HasAnAcuteAngle() const{
	int last = vertices.size() - 1;
	for(int i = 0; i < vertices.size(); ++i){
		double angle;
		if(i == last){
			angle = Angle(vertices[i-1], vertices[i], vertices[0]);
		} else if(i == 0){
			angle = Angle(vertices[last], vertices[0], vertices[1]);
		} else{
			angle = Angle(vertices[i-1], vertices[i], vertices[i+1]);
		}
		if(AcuteAngle(angle) == true){
			return true;
		}
	}
	return false;
}

//================================================================

bool Polygon::IsConvex() const{
	int last = vertices.size() - 1;
	for(int i = 0; i < vertices.size(); ++i){
		double angle;
		if(i == last){
			angle = Angle(vertices[i-1], vertices[i], vertices[0]);
		} else if(i == 0){
			angle = Angle(vertices[last], vertices[0], vertices[1]);
		} else{
			angle = Angle(vertices[i-1], vertices[i], vertices[i+1]);
		}
		if(ReflexAngle(angle) == true){
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
	double angleSum = Angle(v[2], v[0], v[1]) + Angle(v[0], v[1], v[2]) + Angle(v[1], v[2], v[0]);
	if(EqualAngles(angleSum, 180.0) == false){ throw -1; }
}

//================================================================

Quadrilateral::Quadrilateral(const std::string& n, const std::vector<Point>& v) : Polygon(n, v) {
	if(v.size() != 4){ throw -1; }
	double angleSum = Angle(v[3], v[0], v[1]) + Angle(v[0], v[1], v[2]) 
					+ Angle(v[1], v[2], v[3]) + Angle(v[2], v[3], v[0]);
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
	//check for two equal sides, and corresponding equal angles
	double sideAB = DistanceBetween(v[0], v[1]);
	double sideBC = DistanceBetween(v[1], v[2]);
	double sideCA = DistanceBetween(v[2], v[0]);
	double angleCAB = Angle(v[2], v[0], v[1]);
	double angleABC = Angle(v[0], v[1], v[2]);
	double angleBCA = Angle(v[1], v[2], v[0]);

	bool pair1 = EqualSides(sideAB, sideBC) && EqualAngles(angleCAB, angleBCA);
	bool pair2 = EqualSides(sideBC, sideCA) && EqualAngles(angleABC, angleCAB);
	bool pair3 = EqualSides(sideCA, sideAB) && EqualAngles(angleBCA, angleABC);

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

	double sideAB = DistanceBetween(v[0], v[1]);
	double sideBC = DistanceBetween(v[1], v[2]);
	double sideCD = DistanceBetween(v[2], v[3]);
	double sideDA = DistanceBetween(v[3], v[0]);

	bool group1 = EqualSides(sideAB, sideBC) && EqualSides(sideCD, sideDA);
	bool group2 = EqualSides(sideBC, sideCD) && EqualSides(sideDA, sideAB);

	// if(group1 == false && group2 == false){ throw -1; }

	int equalpairs = 0;
	if(group1) equalpairs++;
	if(group2) equalpairs++;
	if(equalpairs < 1) throw -1;
}

//================================================================

Arrow::Arrow(const std::string& n, const std::vector<Point>& v) : Quadrilateral(n, v) {
	if(Polygon::IsConvex() == true){ throw -1; }
	if(Polygon::HasAnAcuteAngle() == false){ throw -1; }

	double sideAB = DistanceBetween(v[0], v[1]);
	double sideBC = DistanceBetween(v[1], v[2]);
	double sideCD = DistanceBetween(v[2], v[3]);
	double sideDA = DistanceBetween(v[3], v[0]);

	bool group1 = EqualSides(sideAB, sideBC) && EqualSides(sideCD, sideDA);
	bool group2 = EqualSides(sideBC, sideCD) && EqualSides(sideDA, sideAB);

	if(group1 == false && group2 == false){ throw -1; }
}

//================================================================

Trapezoid::Trapezoid(const std::string& n, const std::vector<Point>& v) : Quadrilateral(n, v) {
	if(Polygon::IsConvex() == false){ throw -1; }

	Vector vecAB(v[0], v[1]);
	Vector vecBC(v[1], v[2]);
	Vector vecCD(v[2], v[3]);
	Vector vecDA(v[3], v[0]);

	// if(!Parallel(vecAB, vecCD) && !Parallel(vecBC, vecDA)){ throw -1; }
	int count = 0;
	if(Parallel(vecAB, vecCD)) count++;
	if(Parallel(vecBC, vecDA)) count++;
	if(count < 1){ throw -1; }
}

//================================================================

Parallelogram::Parallelogram(const std::string& n, const std::vector<Point>& v) : Quadrilateral(n, v), Trapezoid(n, v) {
	Vector vecAB(v[0], v[1]);
	Vector vecBC(v[1], v[2]);
	Vector vecCD(v[2], v[3]);
	Vector vecDA(v[3], v[0]);
	
	// bool pair1 = Parallel(vecAB, vecCD) && EqualSides(vecAB.Length(), vecCD.Length());
	// bool pair2 = Parallel(vecBC, vecDA) && EqualSides(vecBC.Length(), vecDA.Length());
	// if(pair1 == false || pair2 == false){ throw -1; }
	int count = 0;
	if(Parallel(vecAB, vecCD)) count++;
	if(Parallel(vecBC, vecDA)) count++;
	if(count < 2){ throw -1; }

	double angleDAB = Angle(v[3], v[0], v[1]);
	double angleABC = Angle(v[0], v[1], v[2]);
	double angleBCD = Angle(v[1], v[2], v[3]);
	double angleCDA = Angle(v[2], v[3], v[0]);

	if(EqualAngles(angleDAB, angleBCD) == false){ throw -1; }
	if(EqualAngles(angleABC, angleCDA) == false){ throw -1; }

}

//================================================================

IsoscelesTrapezoid::IsoscelesTrapezoid(const std::string& n, const std::vector<Point>& v) : Quadrilateral(n, v), Trapezoid(n, v){
	// Vector vecAB(v[0], v[1]);
	// Vector vecBC(v[1], v[2]);
	// Vector vecCD(v[2], v[3]);
	// Vector vecDA(v[3], v[0]);
	// bool pair1 = Parallel(vecAB, vecCD) && EqualSides(vecBC.Length(), vecDA.Length());
	// bool pair2 = Parallel(vecBC, vecDA) && EqualSides(vecAB.Length(), vecCD.Length());

	// if(pair1 == false && pair2 == false){ throw -1; }

	double oppAngleSum1 = Angle(v[3], v[0], v[1]) + Angle(v[1], v[2], v[3]);
	double oppAngleSum2 = Angle(v[0], v[1], v[2]) + Angle(v[2], v[3], v[0]);

	if(EqualAngles(oppAngleSum1, 180.0) == false){ throw -1; }
	if(EqualAngles(oppAngleSum2, 180.0) == false){ throw -1; }
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













