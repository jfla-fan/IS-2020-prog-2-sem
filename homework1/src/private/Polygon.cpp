#include "..\public\Polygon.h"
#include <iostream>

// condition on self-intersections
bool Polygon::_check() const
{
	typedef struct { Point end; } vector_t;

#define make_vector_t(p1, p2) vector_t { Point((p2).getX() - (p1).getX(), (p2).getY() - (p1).getY()) }


	// checking z-cross-product
	auto _get_zcrossproduct = [](const vector_t& e1, const vector_t& e2)
			{ return e1.end.getX() * e2.end.getY() - e1.end.getY() * e2.end.getX(); };
	
	int sign = _get_zcrossproduct(make_vector_t(_points.at(0), _points.at(1)),
								make_vector_t(_points.at(1), _points.at(2))) 
					>= 0 ? 1 : -1;
	int _calc;

	for (auto it1 = _points.begin() + 1; it1 != _points.end(); ++it1)
	{
		for (auto it2 = it1 + 1; it2 != _points.end(); ++it2)
		{
			_calc = _get_zcrossproduct(make_vector_t(*(it1 - 1), *it1), make_vector_t(*(it2 - 1), *it2));
			
			if (_calc * sign < 0)
				return false;
		}

	}

#undef make_vector_t
	
	return true;
}


Polygon::Polygon(const std::vector<Point>& v)
	: ClosedPolygonalChain(v)
{
	if (!_check())
		throw;
}

Polygon::Polygon(int size, Point* points)
	: ClosedPolygonalChain(size, points)
{
	if (!_check())
		throw;
}

Polygon::Polygon(std::initializer_list<Point>& points)
	: ClosedPolygonalChain(points)
{ 
	if (!_check())
		throw;
}

Polygon::Polygon(const Polygon& other)
	: ClosedPolygonalChain(other)
{
	if (!_check())
		throw;
}



double Polygon::area() const
{
	double s = 0;

	for (size_t i = 0; i < _points.size() - 1; ++i)
	{
		s += static_cast<double>(_points[i].getX()) * static_cast<double>(_points[i + 1].getY()) -
			 static_cast<double>(_points[i + 1].getX()) * static_cast<double>(_points[i].getY());
	}

	s += static_cast<double>(_points.back().getX()) * static_cast<double>(_points.front().getY()) - 
		 static_cast<double>(_points.front().getX()) * static_cast<double>(_points.back().getY());

	s /= 2;

	return std::abs(s);
}