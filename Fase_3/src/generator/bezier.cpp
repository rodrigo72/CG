#include "bezier.hpp"

Bezier::Bezier(int tesselation, const vector<vector<Point>>& points_per_patch, const std::vector<Point>& points) : Figure(points), points_per_patch(points_per_patch), tesselation(tesselation) {}

void Bezier::generate_points() {

    float u = 0.0f;
    float v = 0.0f;
    float delta = 1.0f / this->tesselation;

    for (vector<Point> patch : this->points_per_patch) {  // 16 pontos
        for (int i = 0; i < this->tesselation; i++, u += delta) {
            for (int j = 0; j < this->tesselation; j++, v += delta) {

                Point a = Point::surface_point(u, v, patch);
                Point b = Point::surface_point(u, v + delta, patch);
                Point c = Point::surface_point(u + delta, v, patch);
                Point d = Point::surface_point(u + delta, v + delta, patch);

                this->points.push_back(c);
                this->points.push_back(a);
                this->points.push_back(b);
                this->points.push_back(b);
                this->points.push_back(d);
                this->points.push_back(c);
            }
            v = 0.0f;
        }
        u = v = 0.0f;
    }
}

Figure::FigureType Bezier::get_type() {
	return Figure::FigureType::BEZIER;
}

Bezier::~Bezier() {}