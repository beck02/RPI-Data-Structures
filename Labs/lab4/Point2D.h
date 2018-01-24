#ifndef Point2D_h_
#define Point2D_h_

class Point2D {

public:
	// constructors
	Point2D() : m_x(0), m_y(0) {}
	Point2D( float x, float y) 
    	: m_x(x), m_y(y) {}
    
	// accessors
	float x() const { return m_x; }
	float y() const { return m_y; }
	
	// modifier
	void set( float x, float y )
    	{ m_x = x; m_y = y; }
    
private:
	// representation
	float m_x, m_y;
};

inline bool operator==(const Point2D& p1, const Point2D& p2)
	{ return ( p1.x() == p2.x() ) && ( p2.y() == p2.y() ); }

#endif