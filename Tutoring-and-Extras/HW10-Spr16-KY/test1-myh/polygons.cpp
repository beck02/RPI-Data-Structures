#include <iostream>
#include <string>
#include <vector>
#include "polygons.h"	

Polygon::Polygon(const std::string& p_name, const std::vector<Point>& p_vertices){
	if (p_vertices.size() < 3)
		throw -1;


	name = p_name;
	vertices = p_vertices;


}
	 bool Polygon::HasAllEqualSides() const {
		double length = DistanceBetween(vertices[0], vertices[1]);
		for (int i = 0; i < vertices.size(); i++){
			if (i == (vertices.size() - 1)){ //checks if first point for equal sides function is the last point in the vector
				double templength = DistanceBetween(vertices[i], vertices[0]);
				if (EqualSides(length, templength) == false) //connects the last point with the first point
					return false;
			}
			else{
				double templength = DistanceBetween(vertices[i], vertices[i + 1]);
				if (EqualSides(length, templength) == false) //connects the last point with the first point
					return false;
			}
		}
		return true;
	}

	 bool Polygon::HasAllEqualAngles() const {
		double angle = Angle(vertices[vertices.size() - 1], vertices[0], vertices[1]); // getting angle of the first point in vector
		for (int i = 1; i < vertices.size(); i++){
			double tempangle;
			if (i == (vertices.size() - 1)){ // if angle to be checked is at the last point in vector
				tempangle = Angle(vertices[i - 1], vertices[i], vertices[0]); 
			}
			else
				tempangle = Angle(vertices[i - 1], vertices[i], vertices[i + 1]);
			if (EqualAngles(angle, tempangle) == false)
				return false;
		}

		return true;
	}


	 bool Polygon::HasARightAngle() const {
		for (int i = 0; i < vertices.size(); i++){
			double angle;
			if (i == (vertices.size() - 1)){ // if angle to be checked is at the last point in vector
				angle = Angle(vertices[i - 1], vertices[i], vertices[0]); 
			}
			else if (i == 0){
				angle = Angle(vertices[vertices.size() - 1], vertices[0], vertices[1]);
			}
			else
				angle = Angle(vertices[i - 1], vertices[i], vertices[i + 1]);
			if (RightAngle(angle) == true)
				return true;
		}

		return false;
	}

	 bool Polygon::HasAnObtuseAngle() const {
		for (int i = 0; i < vertices.size(); i++){
			double angle;
			if (i == (vertices.size() - 1)){ // if angle to be checked is at the last point in vector
				angle = Angle(vertices[i - 1], vertices[i], vertices[0]); 
			}
			else if (i == 0){
				angle = Angle(vertices[vertices.size() - 1], vertices[0], vertices[1]);
			}
			else
				angle = Angle(vertices[i - 1], vertices[i], vertices[i + 1]);
			if (ObtuseAngle(angle) == true)
				return true;
		}

		return false;
	}

	 bool Polygon::HasAnAcuteAngle() const {
		for (int i = 0; i < vertices.size(); i++){
			double angle;
			if (i == (vertices.size() - 1)){ // if angle to be checked is at the last point in vector
				angle = Angle(vertices[i - 1], vertices[i], vertices[0]); 
			}
			else if (i == 0){
				angle = Angle(vertices[vertices.size() - 1], vertices[0], vertices[1]);
			}
			else
				angle = Angle(vertices[i - 1], vertices[i], vertices[i + 1]);
			if (AcuteAngle(angle) == true)
				return true;
		}

		return false;
	}

	 bool Polygon::IsConvex() const {
		for (int i = 0; i < vertices.size(); i++){
			double angle;
			if (i == (vertices.size() - 1)){ // if angle to be checked is at the last point in vector
				angle = Angle(vertices[i - 1], vertices[i], vertices[0]); 
			}
			else if (i == 0){
				angle = Angle(vertices[vertices.size() - 1], vertices[0], vertices[1]);
			}
			else
				angle = Angle(vertices[i - 1], vertices[i], vertices[i + 1]);
			if (ReflexAngle(angle) == true)
				return false;
		}

		return true;
	}

	 bool Polygon::IsConcave() const {
		return !IsConvex();
	}

Triangle::Triangle (const std::string& t_name, const std::vector<Point>& t_vertices) : Polygon(t_name, t_vertices) {
	if (t_vertices.size() != 3)
		throw -1;

	if (Polygon::HasAnAcuteAngle() == false)
		throw -1;

	if (Polygon::IsConvex() == false)
		throw -1;
}

ObtuseTriangle::ObtuseTriangle (const std::string& ot_name, const std::vector<Point>& ot_vertices) : Triangle(ot_name, ot_vertices) {
	if (Polygon::HasAnObtuseAngle() == false)
		throw -1;

	if (Polygon::HasARightAngle() == true)
		throw -1;

	if (Polygon::HasAllEqualSides() == true)
		throw -1;

	if (Polygon::HasAllEqualAngles() == true)
		throw -1;
}

IsoscelesTriangle::IsoscelesTriangle (const std::string& it_name, const std::vector<Point>& it_vertices) : Triangle(it_name, it_vertices) {
	double length1 = DistanceBetween(vertices[0], vertices[1]);
	double length2 = DistanceBetween(vertices[1], vertices[2]);
	double length3 = DistanceBetween(vertices[2], vertices[0]);
	int equalsides = 0;

	if (EqualSides(length1, length2) == true)
		equalsides++;

	if (EqualSides(length2, length3) == true)
		equalsides++;

	if (EqualSides(length3, length1) == true)
		equalsides++;


	if (equalsides < 1) //if triangle doesnt have at least 2 equal angles
		throw -1;
}

RightTriangle::RightTriangle (const std::string& rt_name, const std::vector<Point>& rt_vertices) : Triangle(rt_name, rt_vertices) {
	if (Polygon::HasARightAngle() == false)
		throw -1;

	if (Polygon::HasAllEqualSides() == true)
		throw -1;

	if (Polygon::HasAllEqualAngles() == true)
		throw -1;

	if (Polygon::HasAnObtuseAngle() == true)
		throw -1;
}

IsoscelesObtuseTriangle::IsoscelesObtuseTriangle (const std::string& iot_name, const std::vector<Point>& iot_vertices) : Triangle(iot_name, iot_vertices), IsoscelesTriangle(iot_name, iot_vertices), ObtuseTriangle(iot_name, iot_vertices) {

}

EquilateralTriangle::EquilateralTriangle (const std::string& et_name, const std::vector<Point>& et_vertices) : Triangle(et_name, et_vertices), IsoscelesTriangle(et_name, et_vertices) {
	if (Polygon::HasAllEqualSides() == false)
		throw -1;

	if (Polygon::HasAllEqualAngles() == false)
		throw -1;

	if (Polygon::HasARightAngle() == true)
		throw -1;

	if (Polygon::HasAnObtuseAngle() == true)
		throw -1;
}

IsoscelesRightTriangle::IsoscelesRightTriangle (const std::string& irt_name, const std::vector<Point>& irt_vertices) : Triangle(irt_name, irt_vertices), IsoscelesTriangle(irt_name, irt_vertices), RightTriangle(irt_name, irt_vertices) {

}

Quadrilateral::Quadrilateral (const std::string& q_name, const std::vector<Point>& q_vertices) : Polygon(q_name, q_vertices) {
	if (q_vertices.size() != 4)
		throw -1;
}

Trapezoid::Trapezoid (const std::string& trap_name, const std::vector<Point>& trap_vertices) : Quadrilateral(trap_name, trap_vertices) {
	Vector v1(trap_vertices[0], trap_vertices[1]);
	Vector v2(trap_vertices[1], trap_vertices[2]);
	Vector v3(trap_vertices[2], trap_vertices[3]);
	Vector v4(trap_vertices[3], trap_vertices[0]);

	int count = 0;
	if (Parallel(v1, v3) == true)
		count++;

	if (Parallel(v2, v4) == true)
		count++;

	if (count < 1)
		throw -1;

	if (Polygon::IsConvex() == false)
		throw -1;
}

Kite::Kite (const std::string& k_name, const std::vector<Point>& k_vertices) : Quadrilateral(k_name, k_vertices) {
	double d1 = DistanceBetween(k_vertices[0], k_vertices[1]);
	double d2 = DistanceBetween(k_vertices[1], k_vertices[2]);
	double d3 = DistanceBetween(k_vertices[2], k_vertices[3]);
	double d4 = DistanceBetween(k_vertices[3], k_vertices[0]);


	int equalpairs = 0;
	if (EqualSides(d1, d2) == true && EqualSides(d3, d4) == true)
		equalpairs++;

	if (EqualSides(d2, d3) == true && EqualSides(d1, d4) == true)
		equalpairs++;

	if (equalpairs < 1)
		throw -1;

	if (Polygon::IsConvex() == false)
		throw -1;
}

Arrow::Arrow (const std::string& a_name, const std::vector<Point>& a_vertices) : Quadrilateral(a_name, a_vertices) {
	double d1 = DistanceBetween(a_vertices[0], a_vertices[1]);
	double d2 = DistanceBetween(a_vertices[1], a_vertices[2]);
	double d3 = DistanceBetween(a_vertices[2], a_vertices[3]);
	double d4 = DistanceBetween(a_vertices[3], a_vertices[0]);


	int equalpairs = 0;
	if (EqualSides(d1, d2) == true && EqualSides(d3, d4) == true)
		equalpairs++;

	if (EqualSides(d2, d3) == true && EqualSides(d1, d4) == true)
		equalpairs++;

	if (equalpairs < 1)
		throw -1;

	if (Polygon::IsConvex() == true)
		throw -1;

	if (Polygon::HasAnAcuteAngle() == false)
		throw -1;
}

IsoscelesTrapezoid::IsoscelesTrapezoid (const std::string& itrap_name, const std::vector<Point>& itrap_vertices) : Quadrilateral(itrap_name, itrap_vertices), Trapezoid(itrap_name, itrap_vertices) {
	double angle1 = Angle(vertices[3], vertices[0], vertices[1]);
	double angle2 = Angle(vertices[0], vertices[1], vertices[2]);
	double angle3 = Angle(vertices[1], vertices[2], vertices[3]);
	double angle4 = Angle(vertices[2], vertices[3], vertices[0]);

	if (EqualAngles(angle1 + angle3, 180) == false)
		throw -1;

	if (EqualAngles(angle2 + angle4, 180) == false)
		throw -1; 
} 

Parallelogram::Parallelogram (const std::string& p_name, const std::vector<Point>& p_vertices) : Quadrilateral(p_name, p_vertices), Trapezoid(p_name, p_vertices) {
	double angle1 = Angle(vertices[3], vertices[0], vertices[1]);
	double angle2 = Angle(vertices[0], vertices[1], vertices[2]);
	double angle3 = Angle(vertices[1], vertices[2], vertices[3]);
	double angle4 = Angle(vertices[2], vertices[3], vertices[0]);

	if (EqualAngles(angle1, angle3) == false)
		throw -1;

	if (EqualAngles(angle2, angle4) == false)
		throw -1;

	Vector v1(p_vertices[0], p_vertices[1]);
	Vector v2(p_vertices[1], p_vertices[2]);
	Vector v3(p_vertices[2], p_vertices[3]);
	Vector v4(p_vertices[3], p_vertices[0]);

	int count = 0;
	if (Parallel(v1, v3) == true)
		count++;

	if (Parallel(v2, v4) == true)
		count++;

	if (count < 2)
		throw -1;
} 

Rectangle::Rectangle (const std::string& r_name, const std::vector<Point>& r_vertices) : Quadrilateral(r_name, r_vertices), Trapezoid(r_name, r_vertices), Parallelogram(r_name, r_vertices), IsoscelesTrapezoid(r_name, r_vertices) {
	if (HasARightAngle() == false)
		throw -1;

	if (HasAllEqualAngles() == false)
		throw -1;

	if (HasAnObtuseAngle() == true)
		throw -1;

	if (HasAnAcuteAngle() == false)
		throw -1;
} 

Rhombus::Rhombus (const std::string& rh_name, const std::vector<Point>& rh_vertices) : Quadrilateral(rh_name, rh_vertices), Trapezoid(rh_name, rh_vertices), Parallelogram(rh_name, rh_vertices), Kite(rh_name, rh_vertices) {
	if (HasAllEqualSides() == false)
		throw -1;
}

Square::Square (const std::string& s_name, const std::vector<Point>& s_vertices) : Quadrilateral(s_name, s_vertices), Trapezoid(s_name, s_vertices), Parallelogram(s_name, s_vertices), Rectangle(s_name, s_vertices), Rhombus(s_name, s_vertices) {}



