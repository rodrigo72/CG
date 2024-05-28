#ifndef POINT_HPP
#define POINT_HPP

#include <cmath>
#include <vector>
#include <tuple>

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

    static Point vector_from_two_points(Point a, Point b);
    static std::tuple<Point, Point, Point> surface_point(float u, float v, vector<Point> patch);
    static Point new_sph_point(float a, float b, float radius);
    static Point new_sph_point2(float* center, float a, float radius);
    static Point* cross(Point* a, Point* b);
    static Point cross2(Point a, Point b);
    static Point normalize2(Point p);
};

#endif // POINT_HPP
