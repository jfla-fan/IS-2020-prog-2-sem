#include "..\public\Trapezoid.h"


// trapezoid has the following conditions:
// - bases should not be equal (1)
// - bases should be parallel	 (2)
bool Trapezoid::_check() const
{

	bool condition1 = (_base1 != _base2);
	const Point& p1 = _points.at(0);
	const Point& p2 = _points.at(1);
	const Point& p3 = _points.at(2);
	const Point& p4 = _points.at(3);

	bool condition2 = (((p2.getX() - p3.getX() == 0 && p1.getX() - p4.getX() == 0) ||
		(p2.getY() - p3.getY() == 0 && p1.getY() - p4.getY() == 0) ||
		(p2.getX() - p3.getX()) / (p2.getY() - p3.getY()) == (p1.getX() - p4.getX()) / (p1.getY() - p4.getY())));

	return condition1 && condition2;
}

Trapezoid::Trapezoid(const std::vector<Point>& v)
	: ClosedPolygonalChain(v),
	_base1(Point::distance(v.at(1), v.at(2))),
	_base2(Point::distance(v.at(0), v.at(3)))
{ 
	if (!_check())
		throw;
}

Trapezoid::Trapezoid(int size, Point* points)
	: ClosedPolygonalChain(size, points),
	_base1(Point::distance(points[1], points[2])),
	_base2(Point::distance(points[0], points[3]))
{ 
	if (!_check())
		throw;
}

Trapezoid::Trapezoid(std::initializer_list<Point>& points)
	: ClosedPolygonalChain(points),
	_base1(Point::distance(*(points.begin() + 1), *(points.begin() + 2))),
	_base2(Point::distance(*points.begin(), *(points.begin() + 3)))
{ 
	if (!_check())
		throw;
}

Trapezoid::Trapezoid(const Trapezoid& other)
	: ClosedPolygonalChain(other),
	_base1(Point::distance(other.getPoint(1), other.getPoint(2))),
	_base2(Point::distance(other.getPoint(0), other.getPoint(3)))
{ }

Trapezoid& Trapezoid::operator = (const Trapezoid& another)
{
	if (&another == this)
		return *this;

	__super::operator = (another);

	_base1 = another._base1;
	_base2 = another._base2;


	return *this;
}

double Trapezoid::height() const
{
	double a = _base1;
	double b = _base2;
	double c = Point::distance(_points.at(0), _points.at(1));
	double d = Point::distance(_points.at(2), _points.at(3));

	return sqrt(pow(c, 2) - pow((pow(a - b, 2) + pow(c, 2) - pow(d, 2)) / (2 * (a - b)), 2));
}

inline double Trapezoid::area() const { return height() * (_base1 + _base2) / 2; }

inline int Trapezoid::getN() const { return 4; }
