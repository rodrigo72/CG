#include "point.hpp"

Point::Point() : x(0.0), y(0.0), z(0.0) {}

Point::Point(float x, float y, float z) : x(x), y(y), z(z) {}

Point Point::invertX() { return Point(-1 * x, y, z); }
Point Point::invertY() { return Point(x, -1 * y, z); }
Point Point::invertZ() { return Point(x, y, -1 * z); }
Point Point::new_sph_point(float a, float b, float radius) {
    float x = radius * cosf(b) * sinf(a);
    float y = radius * sinf(b);
    float z = radius * cosf(b) * cosf(a);

    return Point(x, y, z);
}

Point::~Point() {}