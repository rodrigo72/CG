#include "bezier.hpp"

Bezier::Bezier(
    int tesselation, 
    const vector<vector<Point>>& points_per_patch, 
    const vector<Point>& points, 
    const vector<Point>& normals, 
    const vector<Point>& texture_coords
) : Figure(points, normals, texture_coords), points_per_patch(points_per_patch), tesselation(tesselation) {}

void Bezier::generate_points() {

    float u = 0.0f;
    float v = 0.0f;
    float delta = 1.0f / this->tesselation;

    for (vector<Point> patch : this->points_per_patch) {  // 16 pontos
        for (int i = 0; i < this->tesselation; i++, u += delta) {
            for (int j = 0; j < this->tesselation; j++, v += delta) {

                auto a = Point::surface_point(u, v, patch);
                auto b = Point::surface_point(u, v + delta, patch);
                auto c = Point::surface_point(u + delta, v, patch);
                auto d = Point::surface_point(u + delta, v + delta, patch);

                add_point_full(std::get<0>(c), std::get<1>(c), std::get<2>(c));
                add_point_full(std::get<0>(a), std::get<1>(a), std::get<2>(a));
                add_point_full(std::get<0>(b), std::get<1>(b), std::get<2>(b));
                add_point_full(std::get<0>(b), std::get<1>(b), std::get<2>(b));
                add_point_full(std::get<0>(d), std::get<1>(d), std::get<2>(d));
                add_point_full(std::get<0>(c), std::get<1>(c), std::get<2>(c));
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