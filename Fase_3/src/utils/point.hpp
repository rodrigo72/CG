#ifndef POINT_HPP
#define POINT_HPP
#include <cmath>

#include <vector>

using namespace std;

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
    void normalize();

    static Point surface_point(float u, float v, vector<Point> patch);
    static Point new_sph_point(float a, float b, float radius);
    static Point* cross(Point* a, Point* b);
};

#endif // POINT_HPP
