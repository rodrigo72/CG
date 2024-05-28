#include "point.hpp"
#include "matrix.hpp"

Point::Point() : x(0.0), y(0.0), z(0.0) {}

Point::Point(float x, float y, float z) : x(x), y(y), z(z) {}

Point Point::invertX() { return Point(-1 * x, y, z); }
Point Point::invertY() { return Point(x, -1 * y, z); }
Point Point::invertZ() { return Point(x, y, -1 * z); }

static void fcross(const float* a, const float* b, float* res) {
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

std::tuple<Point, Point, Point> Point::surface_point(float u, float v, vector<Point> patch) {
	float M[16] = {
		-1.0f,  3.0f, -3.0f, 1.0f,
		 3.0f, -6.0f,  3.0f, 0.0f,
		-3.0f,  3.0f,  0.0f, 0.0f,
		 1.0f,  0.0f,  0.0f, 0.0f
	};

	float U[4] = { u * u * u,u * u,u,1.0f }, V[4] = { v * v * v,v * v,v,1.0f };
	float DER_U[4] = { 3.0f * u * u, 2.0f * u, 1.0f, 0.0f };
	float DER_V[4] = { 3.0f * v * v, 2.0f * v, 1.0f, 0.0f };

	float UM[4]; multiply_matrices(1, 4, U, 4, 4, M, UM); // UM: 1x4
	float MV[4]; multiply_matrices(4, 4, M, 4, 1, V, MV); // MV: 4x1

	float DuM[4]; multiply_matrices(1, 4, DER_U, 4, 4, M, DuM); // DuM: 1x4, U' x M
	float MDv[4]; multiply_matrices(4, 4, M, 4, 1, DER_V, MDv); // MDv: 4x1, M x V' 

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

	float Du[3]; // derivada parcial u
	float Dv[3]; // derivada parcial v

	for (int i = 0; i < 3; i++) {
		float DuMP[4]; // DuMP: 1x4
		multiply_matrices(1, 4, DuM, 4, 4, P[i], DuMP); // U' x M x P
		multiply_matrices(1, 4, DuMP, 4, 1, MV, Du + i); // U' x M x P x M x V
		float UMP[4]; // UMP: 1x4 
		multiply_matrices(1, 4, UM, 4, 4, P[i], UMP); // U x M x P
		multiply_matrices(1, 4, UMP, 4, 1, MDv, Dv + i); // U x M x P x M x V'
	}

	float normal[3];
	fcross(Dv, Du, normal);

	return std::make_tuple(
		Point(res[0], res[1], res[2]), // point
		Point(normal[0], normal[1], normal[2]), // normal
		Point(1 - v, 1 - u, 0.0f) // texture coord
	);
}

Point Point::new_sph_point(float a, float b, float radius) {
	float x = radius * cosf(b) * sinf(a);
	float y = radius * sinf(b);
	float z = radius * cosf(b) * cosf(a);

	return Point(x, y, z);
}

Point Point::new_sph_point2(float* center, float a, float radius) {
	float x = radius * cos(a) + center[0];
	float y = radius * sin(a) + center[0];
	float z = 0.0f;
	return Point(x, y, z);
}

void Point::normalize() {
	float l = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	this->x = this->x / l;
	this->y = this->y / l;
	this->z = this->z / l;
}

Point Point::normalize2(Point p) {
	float l = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	return Point(
		p.x / l,
		p.y / l,
		p.z / l
	);
}

Point Point::vector_from_two_points(Point a, Point b) {
	Point res;
	res.x = b.x - a.x;
	res.y = b.y - a.y;
	res.z = b.z - a.z;
	return res;
}

Point* Point::cross(Point* a, Point* b) {
	return new Point(
		a->y * b->z - a->z * b->y,
		a->z * b->x - a->x * b->z,
		a->x * b->y - a->y * b->x
	);
}

Point Point::cross2(Point a, Point b) {
	return Point(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

Point::~Point() {}
