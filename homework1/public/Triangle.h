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

	int getN() const override;

	Triangle& operator = (const Triangle& another);

private:
	Point _a, _b, _c;

};

