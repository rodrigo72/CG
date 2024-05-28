#include "transform.hpp"

Transform::Transform(float x, float y, float z, float angle, TransformType type) : x(x), y(y), z(z), angle(angle), type(type) {}
Transform::~Transform() {}