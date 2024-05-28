#include "matrix.hpp"

void multiply_matrices(
	int la, int ca, const float* A,
	int lb, int cb, const float* B,
	float* R, int* lr, int* cr
) {
	if (ca == lb) {
		if (lr) *lr = ca;
		if (cr) *cr = lb;
		for (int i = 0; i < la; i++) {
			for (int j = 0; j < cb; j++) {
				R[i * cb + j] = 0;
				for (int k = 0; k < ca; k++) R[i * cb + j] += A[i * ca + k] * B[k * cb + j];
			}
		}
	}
}

void build_rotation_matrix(Point* a, Point* b, Point* c, float* m) {
	m[0] = a->x; 
	m[1] = a->y;
	m[2] = a->z;
	m[3] = 0;

	m[4] = b->x;
	m[5] = b->y;
	m[6] = b->z;
	m[7] = 0;

	m[8] = c->x;
	m[9] = c->y;
	m[10] = c->z;
	m[11] = 0;

	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

float length(float* a) {
	return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

float dot(float* v1, float* v2) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

float angle_vectors(float* v1, float* v2) {
	float dotProduct = dot(v1, v2);
	float l1 = length(v1);
	float l2 = length(v2);
	return acos(dotProduct / (l1 * l2));
}

void get_catmull_rom_point(float t, Point p0, Point p1, Point p2, Point p3, Point* pos, Point* deriv) {
	// catmull-rom matrix
	float m[16] = { -0.5f,  1.5f, -1.5f,  0.5f,
					 1.0f, -2.5f,  2.0f, -0.5f,
				    -0.5f,  0.0f,  0.5f,  0.0f,
					 0.0f,  1.0f,  0.0f,  0.0f };
	
	// control points
	float P[12] = { p0.x, p0.y, p0.z,
				    p1.x, p1.y, p1.z,
				    p2.x, p2.y, p2.z,
				    p3.x, p3.y, p3.z };

	float A[12]; // 4x3
	multiply_matrices(4, 4, m, 4, 3, P, A);

	float T[4] = { t * t * t, t * t, t, 1 }, DERT[4] = { 3 * t * t, 2 * t, 1, 0 }; // T-> 1x4, DERT -> 1x4

	if (pos) {
		float res_1[3];
		multiply_matrices(1, 4, T, 4, 3, A, res_1);
		pos->x = res_1[0]; 
		pos->y = res_1[1];
		pos->z = res_1[2];
	}
	if (deriv) {
		float res_2[3];
		multiply_matrices(1, 4, DERT, 4, 3, A, res_2);
		deriv->x = res_2[0]; 
		deriv->y = res_2[1]; 
		deriv->z = res_2[2];
	}
}

void get_global_catmull_rom_point(float gt, vector<Point> controlPoints, Point* pos, Point* deriv) {
	size_t POINT_COUNT = controlPoints.size();
	float t = gt * POINT_COUNT;
	int index = (int) floor(t); 
	t = t - index; 

	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;

	get_catmull_rom_point(t, controlPoints[indices[0]], controlPoints[indices[1]], controlPoints[indices[2]], controlPoints[indices[3]], pos, deriv);
}