#pragma once
#include <vector>
#include "Point.h"
#include <numeric>
#include <algorithm>

// polyline should contain at least 1 point

class PolygonalChain : public Shape
{
public:

	explicit PolygonalChain(const std::vector<Point>& v);

	PolygonalChain(int size, Point* points);

	PolygonalChain(std::initializer_list<Point>& points);

	PolygonalChain(const PolygonalChain& other);

	virtual ~PolygonalChain() = default;

	PolygonalChain& operator = (const PolygonalChain& another);

	virtual double perimeter() const;

	inline int getN() const override { return static_cast<int>(_points.size()); }

	Point getPoint(int point_pos) const;

protected:

	std::vector<Point> _points;


};

