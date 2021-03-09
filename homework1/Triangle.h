#pragma once
#include "Point.h"



class Triangle : public Shape
{
public:

	Triangle(const Point& a, const Point& b, const Point& c);

	Triangle(int size, Point* points);

	Triangle(const Triangle& another);

	double perimeter();

	double area();

	bool hasRightAngle() const;

	inline int getN() const override { return 3; }

	Triangle& operator = (const Triangle& another);

private:
	Point _a, _b, _c;

};

