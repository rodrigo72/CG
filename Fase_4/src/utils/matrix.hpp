#include <stdlib.h>
#include <math.h>

#include "point.hpp"

#include <vector>

using namespace std;

void multiply_matrices(int la, int ca, const float* A, int lb, int cb, const float* B, float* R, int* lr = NULL, int* cr = NULL);

void build_rotation_matrix(Point* a, Point* b, Point* c, float* m);

float length(float* a);

float dot(float* v1, float* v2);

float angle_vectors(float* v1, float* v2);

void get_catmull_rom_point(float t, Point p0, Point p1, Point p2, Point p3, Point*pos, Point* deriv);

void get_global_catmull_rom_point(float gt, vector<Point> controll_points, Point* pos, Point* deriv);