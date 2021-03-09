#pragma once
#include "Point.h"
#include "ClosedPolygonalChain.h"


// simple polygon without self-intersections
class Polygon : public ClosedPolygonalChain
{
public:
	
	explicit Polygon(const std::vector<Point>& v);

	Polygon(int size, Point* points);

	Polygon(std::initializer_list<Point>& points);

	Polygon(const Polygon& other);

	virtual double area() const;

protected:

	virtual bool _check() const;

};

