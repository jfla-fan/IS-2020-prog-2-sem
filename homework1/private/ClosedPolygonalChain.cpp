#include "..\public\ClosedPolygonalChain.h"


ClosedPolygonalChain::ClosedPolygonalChain(const std::vector<Point>& v)
	: PolygonalChain(v)
{ }

ClosedPolygonalChain::ClosedPolygonalChain(int size, Point* points)
	: PolygonalChain(size, points)
{ }

ClosedPolygonalChain::ClosedPolygonalChain(std::initializer_list<Point>& points)
	: PolygonalChain(points)
{ }

ClosedPolygonalChain::ClosedPolygonalChain(const ClosedPolygonalChain& other)
	: PolygonalChain(other)
{ }


double ClosedPolygonalChain::perimeter() const
{
	double p = __super::perimeter() + ((getN() > 2) ? Point::distance(_points.front(), _points.back()) : 0);
	return p;
}
