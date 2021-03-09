#pragma once
#include <cmath>
#include "Shape.h"


class Point : public Shape
{
public:

	explicit Point(int x, int y);

	explicit Point();

	Point(const Point& another);

	static double distance(const Point& lhs, const Point& rhs);
	
	inline int getX() const { return _x; }
	inline int getY() const { return _y; }

	inline int getN() const override { return 1; }

	Point& operator = (const Point& p);

private:

	int _x, _y;
};

