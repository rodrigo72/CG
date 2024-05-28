#include "transform.hpp"

#include <vector>

Transform::Transform(float x, float y, float z, float angle, TransformType type) : x(x), y(y), z(z), angle(angle), type(type) {
	this->align = false;
	this->time = 0;
}

Transform::Transform(float time, bool align, std::vector<Point>& points) : x(x), y(y), z(z), time(time), align(align), points(points) {
	this->type = TransformType::TRANSLATE_ANIMATION;
	this->angle = 0;
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
Transform::Transform(float x, float y, float z, float time) : x(x), y(y), z(z), time(time) {
	this->angle = 0;
	this->align = false;
	this->type = TransformType::ROTATE_ANIMATION;
}

Transform::~Transform() {}
