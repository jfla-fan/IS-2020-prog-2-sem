#include "Triangle.h"


Triangle::Triangle(const Point& a, const Point& b, const Point& c)
	: _a(a), _b(b), _c(c)
{ }

Triangle::Triangle(int size, Point* points)
{
	if (size != 3)
		throw;

	_a = points[0];
	_b = points[1];
	_c = points[2];
}

Triangle::Triangle(const Triangle& another)
	: _a(another._a), _b(another._b), _c(another._c)
{ }

double Triangle::perimeter()
{
	return Point::distance(_a, _b) + Point::distance(_b, _c) + Point::distance(_a, _c);
}

// fixed: no heron
double Triangle::area()
{
    int x1 = _a.getX(), y1 = _a.getY();
    int x2 = _b.getX(), y2 = _b.getY();
    int x3 = _c.getX(), y3 = _c.getY();

    return static_cast<double>((x1 - x3) * (y2 - y3) - (y1 - y3) * (x2 - x3)) / 2;
}

//fixed: todo without sqrt
bool Triangle::hasRightAngle() const
{
    typedef struct { Point end; } vector_t;

#define make_vector_t(p1, p2) vector_t { Point((p2).getX() - (p1).getX(), (p2).getY() - (p1).getY()) }
#define dot_product(v1, v2) ((v1).end.getX() * (v2).end.getX() + (v1).end.getY() * (v2).end.getY())

    vector_t v1 = make_vector_t(_a, _b);
    vector_t v2 = make_vector_t(_b, _c);
    vector_t v3 = make_vector_t(_a, _c);

    return dot_product(v1, v2) == 0 ||
            dot_product(v2, v3) == 0 ||
            dot_product(v1, v3) == 0;

#undef make_vector_t
#undef dot_product
}

Triangle& Triangle::operator = (const Triangle& another)
{
	if (&another == this)
		return *this;

	_a = another._a;
	_b = another._b;
	_c = another._c;

	return *this;
}
