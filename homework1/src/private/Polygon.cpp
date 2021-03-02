#include "..\public\Polygon.h"

// condition on self-intersections
bool Polygon::_check() const
{
	return true;
}


Polygon::Polygon(const std::vector<Point>& v)
	: ClosedPolygonalChain(v)
{ }

Polygon::Polygon(int size, Point* points)
	: ClosedPolygonalChain(size, points)
{ }

Polygon::Polygon(std::initializer_list<Point>& points)
	: ClosedPolygonalChain(points)
{ }

Polygon::Polygon(const Polygon& other)
	: ClosedPolygonalChain(other)
{ }



double Polygon::area() const
{
	double s = 0;

	for (int i = 0; i < _points.size() - 1; ++i)
	{
		s += _points[i].getX() * _points[i + 1].getY() - _points[i + 1].getX() * _points[i].getY();
	}

	s += _points.back().getX() * _points.front().getY() - _points.front().getX() * _points.back().getY();

	s /= 2;

	return std::abs(s);
}