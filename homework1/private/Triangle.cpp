#include "..\public\Triangle.h"


Triangle::Triangle(const Point& a, const Point& b, const Point& c)
	: _a(a), _b(b), _c(c)
{ }

Triangle::Triangle(int size, Point* points)
{
	if (size != 3)
		throw;

	_a = points[0];
	_b = points[1];
	_c = points[2];
}

Triangle::Triangle(const Triangle& another)
	: _a(another._a), _b(another._b), _c(another._c)
{ }

double Triangle::perimeter()
{
	return Point::distance(_a, _b) + Point::distance(_b, _c) + Point::distance(_a, _c);
}

double Triangle::area()
{
	//no Heron's
	double a = Point::distance(_a, _b);
	double b = Point::distance(_b, _c);
	double c = Point::distance(_a, _c);

	double p = (a + b + c) / 2; // semi-perimeter

	return std::sqrt(p * (p - a) * (p - c) * (p - c));
}

//todo without sqrt
bool Triangle::hasRightAngle() const
{
	double a = Point::distance(_a, _b);
	double b = Point::distance(_b, _c);
	double c = Point::distance(_a, _c);

	return
		(a * a + b * b == c * c) ||
		(b * b + c * c == a * a) ||
		(a * a + c * c == b * b);
}

int Triangle::getN() const
{
	return 3;
}

Triangle& Triangle::operator = (const Triangle& another)
{
	if (&another == this)
		return *this;

	_a = another._a;
	_b = another._b;
	_c = another._c;

	return *this;
}
