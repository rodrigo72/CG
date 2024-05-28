#include "plane.hpp"

Plane::Plane(
    int length, 
    int divisions, 
    int height, 
    const std::vector<Point>& points, 
    const std::vector<Point>& normals, 
    const std::vector<Point>& texture_coords
) : length(length), divisions(divisions), height(height), Figure(points, normals, texture_coords) {}

Plane::~Plane() {}

void Plane::generate_points() {
    points.clear();

    float dimension2 = static_cast<float>(length) / 2;
    float div_side = static_cast<float>(length) / divisions;
    float f_height = static_cast<float>(height);

    Point normal(0.0f, 1.0f, 0.0f);
    for (int line = 0; line < divisions; line++) {
        float z1 = -dimension2 + line * div_side;
        float z2 = z1 + div_side;

        float t_z1 = (line * div_side) / length;
        float t_z2 = ((line + 1) * div_side) / length;

        for (int column = 0; column < divisions; column++) {
            float x1 = -dimension2 + column * div_side;
            float x2 = x1 + div_side;
            
            float t_x1 = (column * div_side) / length;
            float t_x2 = ((column + 1) * div_side) / length;

            // 1 -- 3
            // | \  |
            // |  \ |
            // 2 -- 4

            // First triangle
            add_point_full(Point(x1, f_height, z1), normal, Point(t_x1, t_z1, 0.0f)); // 1
            add_point_full(Point(x1, f_height, z2), normal, Point(t_x1, t_z2, 0.0f)); // 2
            add_point_full(Point(x2, f_height, z2), normal, Point(t_x2, t_z2, 0.0f)); // 4

            // Second triangle
            add_point_full(Point(x2, f_height, z2), normal, Point(t_x2, t_z2, 0.0f)); // 4
            add_point_full(Point(x2, f_height, z1), normal, Point(t_x2, t_z1, 0.0f)); // 3
            add_point_full(Point(x1, f_height, z1), normal, Point(t_x1, t_z1, 0.0f)); // 1
        }
    }
}

Figure::FigureType Plane::get_type() {
    return Figure::FigureType::PLANE;
}