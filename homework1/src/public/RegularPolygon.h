#pragma once
#include "Polygon.h"
#include "Point.h"



class RegularPolygon : public Polygon
{
public:

	RegularPolygon(int size, Point* points);

	RegularPolygon(const std::vector<Point>& points);

	RegularPolygon(std::initializer_list<Point>& points);

	RegularPolygon(const RegularPolygon& other);

	double perimeter() const override;

protected:

	double _sideLength;

	virtual bool _check() const override;
	
};

