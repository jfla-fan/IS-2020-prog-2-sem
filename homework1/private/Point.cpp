#include "../public/Point.h"


Point::Point(int x, int y)
	: _x(x), _y(y)
{ }

Point::Point()
	: _x(0), _y(0)
{ }


Point::Point(const Point& another)
{
	_x = another._x;
	_y = another._y;
}

Point& Point::operator = (const Point& p)
{
	if (&p == this)
		return *this;

	_x = p._x;
	_y = p._y;

	return *this;
}

double Point::distance(const Point& lhs, const Point& rhs)
{
	return std::sqrt(std::pow(rhs._x - lhs._x, 2) + std::pow(rhs._y - lhs._y, 2));
}

inline int Point::getN() const { return 1; }