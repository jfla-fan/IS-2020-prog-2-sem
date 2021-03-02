#include "..\public\PolygonalChain.h"


PolygonalChain::PolygonalChain(const std::vector<Point>& v)
	: _points(v)
{ 
	if (_points.size() == 0)
		throw;
}

PolygonalChain::PolygonalChain(std::initializer_list<Point>& points)
	: _points(points.begin(), points.end())
{
	if (_points.size() == 0)
		throw;
}

PolygonalChain::PolygonalChain(const PolygonalChain& other)
	: _points(other._points)
{ }

PolygonalChain::PolygonalChain(int size, Point* points)
	: _points(points, points + size)
{
	if (size == 0)
		throw;
}

PolygonalChain& PolygonalChain::operator = (const PolygonalChain& another)
{
	if (&another == this)
		return *this;

	_points = another._points;

	return *this;
}


double PolygonalChain::perimeter() const
{
	if (_points.size() == 0)
		return 0;

	double p = 0;
	for (auto it = _points.begin() + 1; it != _points.end(); ++it)
	{
		p += Point::distance(*it, *(it - 1));
	}

	return p;
}

inline int PolygonalChain::getN() const { return static_cast<int>(_points.size()); }

Point PolygonalChain::getPoint(int point_pos) const { return _points.at(point_pos); }

