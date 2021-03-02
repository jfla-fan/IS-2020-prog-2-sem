#pragma once
#include <vector>
#include "Point.h"
#include <numeric>
#include <algorithm>

// polyline should contain at least 1 point

class PolygonalChain : public Shape
{
public:

	PolygonalChain(const std::vector<Point>& v);

	PolygonalChain(int size, Point* points);

	PolygonalChain(std::initializer_list<Point>& points);

	PolygonalChain(const PolygonalChain& other);

	PolygonalChain& operator = (const PolygonalChain& another);

	virtual double perimeter() const;

	int getN() const override;

	Point getPoint(int point_pos) const;

protected:

	std::vector<Point> _points;


};

