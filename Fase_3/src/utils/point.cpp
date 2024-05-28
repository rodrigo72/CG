#include "point.hpp"
#include "matrix.hpp"

Point::Point() : x(0.0), y(0.0), z(0.0) {}

Point::Point(float x, float y, float z) : x(x), y(y), z(z) {}

Point Point::invertX() { return Point(-1 * x, y, z); }
Point Point::invertY() { return Point(x, -1 * y, z); }
Point Point::invertZ() { return Point(x, y, -1 * z); }

Point Point::surface_point(float u, float v, vector<Point> patch) {
	float M[16] = {
		-1.0f,  3.0f, -3.0f, 1.0f,
		 3.0f, -6.0f,  3.0f, 0.0f,
		-3.0f,  3.0f,  0.0f, 0.0f,
		 1.0f,  0.0f,  0.0f, 0.0f
	};


	float U[4] = { u * u * u,u * u,u,1.0f }, V[4] = { v * v * v,v * v,v,1.0f };
	float UM[4]; multiply_matrices(1, 4, U, 4, 4, M, UM); // UM: 1x4
	float MV[4]; multiply_matrices(4, 4, M, 4, 1, V, MV); // MV: 4x1

	float P[3][16] = {
		{
			patch[0].x,  patch[1].x,  patch[2].x,  patch[3].x,
			patch[4].x,  patch[5].x,  patch[6].x,  patch[7].x,
			patch[8].x,  patch[9].x,  patch[10].x, patch[11].x,
			patch[12].x, patch[13].x, patch[14].x, patch[15].x
		},
		{
			patch[0].y,  patch[1].y,  patch[2].y,  patch[3].y,
			patch[4].y,  patch[5].y,  patch[6].y,  patch[7].y,
			patch[8].y,  patch[9].y,  patch[10].y, patch[11].y,
			patch[12].y, patch[13].y, patch[14].y, patch[15].y
		},
		{
			patch[0].z,  patch[1].z,  patch[2].z,  patch[3].z,
			patch[4].z,  patch[5].z,  patch[6].z,  patch[7].z,
			patch[8].z,  patch[9].z,  patch[10].z, patch[11].z,
			patch[12].z, patch[13].z, patch[14].z, patch[15].z
		}
	};

	float res[3];

	for (int i = 0; i < 3; i++) {
		float UMP[4];
		multiply_matrices(1, 4, UM, 4, 4, P[i], UMP);
		multiply_matrices(1, 4, UMP, 4, 1, MV, &res[i]);
	}

	return Point(res[0], res[1], res[2]);
}

Point Point::new_sph_point(float a, float b, float radius) {
	float x = radius * cosf(b) * sinf(a);
	float y = radius * sinf(b);
	float z = radius * cosf(b) * cosf(a);

	return Point(x, y, z);
}

void Point::normalize() {
	float l = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	this->x = this->x / l;
	this->y = this->y / l;
	this->z = this->z / l;
}

Point* Point::cross(Point* a, Point* b) {
	return new Point(
		a->y * b->z - a->z * b->y,
		a->z * b->x - a->x * b->z,
		a->x * b->y - a->y * b->x
	);
}

Point::~Point() {}
