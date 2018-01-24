#include <iostream>
#include <string>
#include <vector>
 
#include "utilities.h"
 
class Polygon {
public:
	Polygon (const std::string& p_name, const std::vector<Point>& p_vertices);
	virtual ~Polygon() {}
	const std::string& getName() const {return name;}
	virtual  bool HasAllEqualSides() const;
	virtual  bool HasAllEqualAngles() const;
	virtual  bool HasARightAngle() const;
	virtual  bool HasAnObtuseAngle() const;
	virtual  bool HasAnAcuteAngle() const;
	virtual  bool IsConvex() const;
	virtual  bool IsConcave() const;

protected:
	std::string name;
	std::vector<Point> vertices;
};



class Triangle : public Polygon {
public:
	Triangle (const std::string& t_name, const std::vector<Point>& t_vertices);

	 bool HasAnAcuteAngle() const {return true;}
	 bool IsConvex() const {return true;}
	 bool IsConcave() const {return false;}

};

class ObtuseTriangle : public virtual Triangle {
public:
	ObtuseTriangle (const std::string& ot_name, const std::vector<Point>& ot_vertices);

	 bool HasAnObtuseAngle() const {return true;}
	 bool HasAllEqualSides() const {return false;}
	 bool HasAllEqualAngles() const {return false;}
	 bool HasARightAngle() const {return false;}

};

class IsoscelesTriangle : public virtual Triangle {
public:
	IsoscelesTriangle (const std::string& it_name, const std::vector<Point>& it_vertices);
};

class RightTriangle : public virtual Triangle {
public:
	RightTriangle (const std::string& rt_name, const std::vector<Point>& rt_vertices);

	 bool HasARightAngle() const {return true;}
	 bool HasAnObtuseAngle() const {return false;}
	 bool HasAllEqualSides() const {return false;}
	 bool HasAllEqualAngles() const {return false;}
};

class IsoscelesObtuseTriangle : public IsoscelesTriangle, public ObtuseTriangle {
public:
	IsoscelesObtuseTriangle (const std::string& iot_name, const std::vector<Point>& iot_vertices);
};

class EquilateralTriangle : public IsoscelesTriangle {
public:
	EquilateralTriangle (const std::string& et_name, const std::vector<Point>& et_vertices);

	 bool HasAllEqualSides() const {return true;}
	 bool HasAllEqualAngles() const {return true;}
	 bool HasARightAngle() const {return false;}
	 bool HasAnObtuseAngle() const {return false;}
};

class IsoscelesRightTriangle : public IsoscelesTriangle, public RightTriangle {
public:
	IsoscelesRightTriangle (const std::string& irt_name, const std::vector<Point>& irt_vertices);

};

class Quadrilateral : public Polygon {
public:
	Quadrilateral (const std::string& q_name, const std::vector<Point>& q_vertices);
};

class Trapezoid : public virtual Quadrilateral {
public:
	Trapezoid (const std::string& trap_name, const std::vector<Point>& trap_vertices);

	 bool IsConvex() const {return true;}
	 bool IsConcave() const {return false;}
};

class Kite : public virtual Quadrilateral {
public:
	Kite (const std::string& k_name, const std::vector<Point>& k_vertices);

	//const bool IsConvex() const {return true;}
	//const bool IsConcave() const {return false;}
};

class Arrow : public virtual Quadrilateral {
public:
	Arrow (const std::string& a_name, const std::vector<Point>& a_vertices); 

	 bool IsConvex() const {return false;}
	 bool IsConcave() const {return true;}
	 bool HasAnAcuteAngle() const {return true;}
};

class IsoscelesTrapezoid : public virtual Trapezoid {
public:
	IsoscelesTrapezoid (const std::string& itrap_name, const std::vector<Point>& itrap_vertices); 
};

class Parallelogram : public virtual Trapezoid {
public: 
	Parallelogram (const std::string& p_name, const std::vector<Point>& p_vertices); 
};

class Rectangle : public IsoscelesTrapezoid, public virtual Parallelogram {
public:
	Rectangle (const std::string& r_name, const std::vector<Point>& r_vertices);

	 bool HasARightAngle() const {return true;}
	 bool HasAllEqualAngles() const {return true;}
	 bool HasAnObtuseAngle() const {return false;}
	 bool HasAnAcuteAngle() const {return false;} 
};

class Rhombus : public Kite, public virtual Parallelogram {
public:
	Rhombus (const std::string& rh_name, const std::vector<Point>& rh_vertices);

	 bool HasAllEqualSides() const {return true;}
	//const bool IsConcave() const {return false;}
	//const bool IsConvex() const {return true;}
};

class Square : public Rectangle, public Rhombus {
public:
	Square (const std::string& s_name, const std::vector<Point>& s_vertices);
};















