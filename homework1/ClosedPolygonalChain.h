#pragma once
#include "PolygonalChain.h"




class ClosedPolygonalChain : public PolygonalChain
{
public:

	explicit ClosedPolygonalChain(const std::vector<Point>& v);

	ClosedPolygonalChain(int size, Point* points);

	ClosedPolygonalChain(std::initializer_list<Point>& points);

//	ClosedPolygonalChain(const ClosedPolygonalChain& other);

	double perimeter() const override;
};

