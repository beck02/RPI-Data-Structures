#include <iostream>
#include <vector>
#include "Point2D.h"
#include "Rectangle.h"
using namespace std;

int main(){
	Point2D p1(0, 0);
	Point2D p2(1, 1);
	Point2D p3(3, 3);
	Point2D p4(4, 2);
	Point2D p5(2, 4);
	Point2D p6(5, 5);
	Point2D p7(6, 8);
	Point2D p8(8, 6);
	Point2D p9(9, 9);
	
	cout << "Points: " << endl;
	cout << "p1: " << p1.x() << "," << p1.y() << endl;
	cout << "p2: " << p2.x() << "," << p2.y() << endl;
	cout << "p3: " << p3.x() << "," << p3.y() << endl;
	cout << "p4: " << p4.x() << "," << p4.y() << endl;
	cout << "p5: " << p5.x() << "," << p5.y() << endl;
	cout << "p6: " << p6.x() << "," << p6.y() << endl;
	cout << "p7: " << p7.x() << "," << p7.y() << endl;
	cout << "p8: " << p8.x() << "," << p8.y() << endl;
	cout << "p9: " << p9.x() << "," << p9.y() << endl;

	p1.set(5, 0);

	Rectangle r1(p1, p3);
	cout << "\nr1(p1, p3): ";
	cout << r1.lower_left_corner().x() << "," << r1.lower_left_corner().y() << "--"
		 << r1.upper_right_corner().x() << "," << r1.upper_right_corner().y() << endl;
	if( r1.is_point_within(p1) ){ r1.add_point(p1); }
	if( r1.is_point_within(p2) ){ r1.add_point(p2); }
	if( r1.is_point_within(p3) ){ r1.add_point(p3); }
	if( r1.is_point_within(p4) ){ r1.add_point(p4); }
	if( r1.is_point_within(p5) ){ r1.add_point(p5); }
	if( r1.is_point_within(p6) ){ r1.add_point(p6); }
	if( r1.is_point_within(p7) ){ r1.add_point(p7); }
	if( r1.is_point_within(p8) ){ r1.add_point(p8); }
	if( r1.is_point_within(p9) ){ r1.add_point(p9); }
	print_rectangle(r1);

	Rectangle r2(p2, p4);
	cout << "\nr2(p2, p4): ";
	cout << r2.lower_left_corner().x() << "," << r2.lower_left_corner().y() << "--"
		 << r2.upper_right_corner().x() << "," << r2.upper_right_corner().y() << endl;
	if( r2.is_point_within(p1) ){ r2.add_point(p1); }
	if( r2.is_point_within(p2) ){ r2.add_point(p2); }
	if( r2.is_point_within(p3) ){ r2.add_point(p3); }
	if( r2.is_point_within(p4) ){ r2.add_point(p4); }
	if( r2.is_point_within(p5) ){ r2.add_point(p5); }
	if( r2.is_point_within(p6) ){ r2.add_point(p6); }
	if( r2.is_point_within(p7) ){ r2.add_point(p7); }
	if( r2.is_point_within(p8) ){ r2.add_point(p8); }
	if( r2.is_point_within(p9) ){ r2.add_point(p9); }
	print_rectangle(r2);

	Rectangle r3(p3, p1);
	cout << "\nr3(p3, p1): ";
	cout << r3.lower_left_corner().x() << "," << r3.lower_left_corner().y() << "--"
		 << r3.upper_right_corner().x() << "," << r3.upper_right_corner().y() << endl;
	if( r3.is_point_within(p1) ){ r3.add_point(p1); }
	if( r3.is_point_within(p2) ){ r3.add_point(p2); }
	if( r3.is_point_within(p3) ){ r3.add_point(p3); }
	if( r3.is_point_within(p4) ){ r3.add_point(p4); }
	if( r3.is_point_within(p5) ){ r3.add_point(p5); }
	if( r3.is_point_within(p6) ){ r3.add_point(p6); }
	if( r3.is_point_within(p7) ){ r3.add_point(p7); }
	if( r3.is_point_within(p8) ){ r3.add_point(p8); }
	if( r3.is_point_within(p9) ){ r3.add_point(p9); }
	print_rectangle(r3);
	
	Rectangle r4(p5, p8);
	cout << "\nr4(p5, p8): ";
	cout << r4.lower_left_corner().x() << "," << r4.lower_left_corner().y() << "--"
		 << r4.upper_right_corner().x() << "," << r4.upper_right_corner().y() << endl;
	if( r4.is_point_within(p1) ){ r4.add_point(p1); }
	if( r4.is_point_within(p2) ){ r4.add_point(p2); }
	if( r4.is_point_within(p3) ){ r4.add_point(p3); }
	if( r4.is_point_within(p4) ){ r4.add_point(p4); }
	if( r4.is_point_within(p5) ){ r4.add_point(p5); }
	if( r4.is_point_within(p6) ){ r4.add_point(p6); }
	if( r4.is_point_within(p7) ){ r4.add_point(p7); }
	if( r4.is_point_within(p8) ){ r4.add_point(p8); }
	if( r4.is_point_within(p9) ){ r4.add_point(p9); }
	print_rectangle(r4);

	Rectangle r5(p6, p9);
	cout << "\nr5(p6, p9): ";
	cout << r5.lower_left_corner().x() << "," << r5.lower_left_corner().y() << "--"
		 << r5.upper_right_corner().x() << "," << r5.upper_right_corner().y() << endl;
	if( r5.is_point_within(p1) ){ r5.add_point(p1); }
	if( r5.is_point_within(p2) ){ r5.add_point(p2); }
	if( r5.is_point_within(p3) ){ r5.add_point(p3); }
	if( r5.is_point_within(p4) ){ r5.add_point(p4); }
	if( r5.is_point_within(p5) ){ r5.add_point(p5); }
	if( r5.is_point_within(p6) ){ r5.add_point(p6); }
	if( r5.is_point_within(p7) ){ r5.add_point(p7); }
	if( r5.is_point_within(p8) ){ r5.add_point(p8); }
	if( r5.is_point_within(p9) ){ r5.add_point(p9); }
	print_rectangle(r5);

	vector<Point2D> v1 = points_in_both(r1, r2);
	cout << "\nPoints in rectangles r1 and r2:" << endl;
	for (int i = 0; i < v1.size(); ++i){
		cout << v1[i].x() << "," << v1[i].y() << endl;
	}
	vector<Point2D> v2 = points_in_both(r2, r3);
	cout << "\nPoints in rectangles r2 and r3:" << endl;
	for (int i = 0; i < v2.size(); ++i){
		cout << v2[i].x() << "," << v2[i].y() << endl;
	}
	vector<Point2D> v3 = points_in_both(r3, r4);
	cout << "\nPoints in rectangles r3 and r4:" << endl;
	for (int i = 0; i < v3.size(); ++i){
		cout << v3[i].x() << "," << v3[i].y() << endl;
	}
	vector<Point2D> v4 = points_in_both(r4, r5);
	cout << "\nPoints in rectangles r4 and r5:" << endl;
	for (int i = 0; i < v4.size(); ++i){
		cout << v4[i].x() << "," << v4[i].y() << endl;
	}
	vector<Point2D> v5 = points_in_both(r2, r4);
	cout << "\nPoints in rectangles r2 and r4:" << endl;
	for (int i = 0; i < v5.size(); ++i){
		cout << v5[i].x() << "," << v5[i].y() << endl;
	}


	
	return 0;
}