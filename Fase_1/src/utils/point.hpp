#ifndef POINT_HPP
#define POINT_HPP

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
};

#endif // POINT_HPP
