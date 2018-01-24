#include <iostream>
#include "Rectangle.h"


Rectangle::Rectangle(const Point2D& lower_left, const Point2D& upper_right)
	: m_upper_right(upper_right), m_lower_left(lower_left)
	{ }


bool Rectangle::is_point_within(const Point2D& p) const {
	return
		(p.x() <= m_upper_right.x() && p.y() <= m_upper_right.y()) ||
		(p.x() >= m_lower_left.x() && p.y() >= m_lower_left.y());
}


std::vector<Point2D> points_in_both(const Rectangle& r1, const Rectangle& r2) {
	//  Use a reference/alias to access the vector of points in the two
	//  rectangles ** without copying ** these vectors.  Instead,
	//  r1_points and r2_points are references to the vectors of points,
	//  but since they are constants neither the vectors nor the points
	//  within them can be changed.
	const std::vector<Point2D> & r1_points = r1.points_contained();
	const std::vector<Point2D> & r2_points = r2.points_contained();
	
	std::vector<Point2D> pts_in_both;
	for(int i = 0; i < r1_points.size(); i++){
		for(int k = 0; k < r2_points.size(); k++){
			if( r1_points[i] == r2_points[k] ){
				pts_in_both.push_back( r1_points[i] );
				break;
			}
		}
	}
	return pts_in_both;
}

bool Rectangle::add_point(const Point2D& p){
	bool ret = false;
	int i;
	for(i = 0; i < m_points_contained.size(); i++){
		if( p == m_points_contained[i] ){
			ret = false;
			break;
		}
	}
	if( i == m_points_contained.size() ){
		m_points_contained.push_back(p);
		ret = true;
	}
	return ret;
}

void print_rectangle(const Rectangle& r1) {
	
	// get all the points that are inside the rectangle
	const std::vector<Point2D> & r1_points = r1.points_contained();
	
	// print the rectangle coordinates
	std::cout << "Rectangle: " 
		<< r1.lower_left_corner().x() << "," << r1.lower_left_corner().y() << "--"
		<< r1.upper_right_corner().x() << "," << r1.lower_left_corner().y() << std::endl;
  
	// print points that are inside
	std::cout << "Points inside:" << std::endl;
	for( std::vector<Point2D>::size_type i = 0; i < r1_points.size(); ++i ) {
		std::cout << r1_points[i].x() << "," << r1_points[i].y() << std::endl;
	}
}

