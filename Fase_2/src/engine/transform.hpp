#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP


class Transform {
public:
    enum TransformType { TRANSLATE = 1, SCALE = 2, ROTATE = 3 };

    Transform(float x, float y, float z, float angle, TransformType type);
    ~Transform();

    float x, y, z, angle;
    TransformType type;
};

#endif // TRANSFORM_HPP
