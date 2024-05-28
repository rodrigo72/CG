#ifndef POINT_HPP
#define POINT_HPP
#include <cmath>
class Point {
public:
    float x;
    float y;
    float z;

    Point();
    Point(float x, float y, float z);
    ~Point();

    Point invertX();
    Point invertY();
    Point invertZ();
    static Point new_sph_point(float a, float b, float radius);
};

#endif // POINT_HPP
