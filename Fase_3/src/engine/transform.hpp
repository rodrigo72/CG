#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "../utils/point.hpp"
#include <vector>

class Transform {
public:
    enum TransformType { TRANSLATE = 1, SCALE = 2, ROTATE = 3, TRANSLATE_ANIMATION = 4, ROTATE_ANIMATION = 5 };

    Transform(float x, float y, float z, float angle, TransformType type);
    Transform(float time, bool align, std::vector<Point>& points);  // TRANSLATE_ANIMATION
    Transform(float x, float y, float z, float time);  // ROTATE_ANIMATION
    ~Transform();

    float x, y, z, angle;
    TransformType type;

    bool align;
    float time;
    std::vector<Point> points;
    Point* y_axis = new Point(0, 1, 0);
};

#endif // TRANSFORM_HPP
