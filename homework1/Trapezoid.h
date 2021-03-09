#pragma once
#include "ClosedPolygonalChain.h"


// bases should be parallel
// should contain only 4 points

class Trapezoid : public ClosedPolygonalChain
{
public:
	
	explicit Trapezoid(const std::vector<Point>& v);

	Trapezoid(int size, Point* points);

	Trapezoid(std::initializer_list<Point>& points);

	Trapezoid(const Trapezoid& other);
	
    virtual ~Trapezoid() = default;

	Trapezoid& operator = (const Trapezoid& another);

	double height() const;

	inline int getN() const override { return 4; }

	inline double area() const;

private:

	double _base1;
	double _base2;

protected:
	bool _check() const;
};

