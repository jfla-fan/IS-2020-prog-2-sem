#include "..\public\RegularPolygon.h"


bool RegularPolygon::_check() const
{
	bool condition1 = true;
	for (auto it = _points.begin() + 1; it != _points.end() && condition1; ++it)
	{
		condition1 = condition1 && (Point::distance(*(it - 1), *it) == _sideLength);
	}

	bool condition2 = (Point::distance(_points.front(), _points.back()) == _sideLength);

	return condition1 && condition2;
}

RegularPolygon::RegularPolygon(int size, Point* points)
	: Polygon(size, points),
	_sideLength(Point::distance(points[0], points[1]))
{ 
	if (!_check())
		throw;
}

RegularPolygon::RegularPolygon(const std::vector<Point>& points)
	: Polygon(points),
	_sideLength(Point::distance(points.at(0), points.at(0)))
{
	if (!_check())
		throw;
}

RegularPolygon::RegularPolygon(std::initializer_list<Point>& points)
	: Polygon(points),
	_sideLength(Point::distance(*points.begin(), *(points.begin() + 1)))
{
	if (!_check())
		throw;
}

RegularPolygon::RegularPolygon(const RegularPolygon& other)
	: Polygon(other),
	_sideLength(other._sideLength)
{ }


double RegularPolygon::perimeter() const
{
	return _sideLength * getN();
}


