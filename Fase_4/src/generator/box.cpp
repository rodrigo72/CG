#include "box.hpp"
#include "plane.hpp"

Box::Box(
    int length, 
    int grid, 
    const std::vector<Point>& points, 
    const std::vector<Point>& normals, 
    const std::vector<Point>& texture_coords
) : length(length), grid(grid), Figure(points, normals, texture_coords) {}

Box::~Box() {}

void Box::generate_points() {
	points.clear();

    float dimension2 = static_cast<float>(length) / 2;
    float div_side = static_cast<float>(length) / grid;

    Point normal = Point(0.0f, -1.0f, 0.0f);
    // Bottom face (reversed plane)
    for (int line = 0; line < grid; line++) {
        float z1 = -dimension2 + line * div_side;
        float z2 = z1 + div_side;

        float t_z1 = (line * div_side) / length;
        float t_z2 = ((line + 1) * div_side) / length;

        for (int column = 0; column < grid; column++) {
            float x1 = -dimension2 + column * div_side;
            float x2 = x1 + div_side;

            float t_x1 = (column * div_side) / length;
            float t_x2 = ((column + 1) * div_side) / length;

            // 1 -- 3
            // | \  |
            // |  \ |
            // 2 -- 4

            // First triangle
            add_point_full(Point(x1, -dimension2, z1), normal, Point(t_x1, t_z1, 0.0f)); // 1
            add_point_full(Point(x2, -dimension2, z1), normal, Point(t_x2, t_z1, 0.0f)); // 3
            add_point_full(Point(x2, -dimension2, z2), normal, Point(t_x2, t_z2, 0.0f)); // 2

            // Second triangle
            add_point_full(Point(x2, -dimension2, z2), normal, Point(t_x2, t_z2, 0.0f)); // 3
            add_point_full(Point(x1, -dimension2, z2), normal, Point(t_x1, t_z2, 0.0f)); // 4
            add_point_full(Point(x1, -dimension2, z1), normal, Point(t_x1, t_z1, 0.0f)); // 2

        }
    }

    normal = Point(0.0f, 1.0f, 0.0f);
    // Top face (normal plane)
    for (int line = 0; line < grid; line++) {
        float z1 = -dimension2 + line * div_side;
        float z2 = z1 + div_side;

        float t_z1 = (line * div_side) / length;
        float t_z2 = ((line + 1) * div_side) / length;

        for (int column = 0; column < grid; column++) {
            float x1 = -dimension2 + column * div_side;
            float x2 = x1 + div_side;

            float t_x1 = (column * div_side) / length;
            float t_x2 = ((column + 1) * div_side) / length;

            // 1 -- 3
            // | \  |
            // |  \ |
            // 2 -- 4

            // First triangle
            add_point_full(Point(x1, dimension2, z1), normal, Point(t_x1, t_z1, 0.0f)); // 1
            add_point_full(Point(x1, dimension2, z2), normal, Point(t_x1, t_z2, 0.0f)); // 2
            add_point_full(Point(x2, dimension2, z2), normal, Point(t_x2, t_z2, 0.0f)); // 4

            // Second triangle
            add_point_full(Point(x2, dimension2, z2), normal, Point(t_x2, t_z2, 0.0f)); // 4
            add_point_full(Point(x2, dimension2, z1), normal, Point(t_x2, t_z1, 0.0f)); // 3
            add_point_full(Point(x1, dimension2, z1), normal, Point(t_x1, t_z1, 0.0f)); // 1
        }
    }

    normal = Point(0.0f, 0.0f, 1.0f);
    // Front face
    for (int line = 0; line < grid; line++) {
        float x1 = -dimension2  + line * div_side;
        float x2 = x1 + div_side;

        float t_x1 = (line * div_side) / length;
        float t_x2 = ((line + 1) * div_side) / length;

        for (int collumn = 0; collumn < grid; collumn++) {
            float y1 = -dimension2  + collumn * div_side;
            float y2 = y1 + div_side;

            float t_y1 = (collumn * div_side) / length;
            float t_y2 = ((collumn + 1) * div_side) / length;

            // Triangle 1
            add_point_full(Point(x1, y1, dimension2), normal, Point(t_x1, t_y1, 0.0f));
            add_point_full(Point(x2, y1, dimension2), normal, Point(t_x2, t_y1, 0.0f));
            add_point_full(Point(x1, y2, dimension2), normal, Point(t_x1, t_y2, 0.0f));

            // Triangle 2
            add_point_full(Point(x2, y2, dimension2), normal, Point(t_x2, t_y2, 0.0f));
            add_point_full(Point(x1, y2, dimension2), normal, Point(t_x1, t_y2, 0.0f));
            add_point_full(Point(x2, y1, dimension2), normal, Point(t_x2, t_y1, 0.0f));
        }
    }

    normal = Point(0.0f, 0.0f, -1.0f);
    // Back face
    for (int line = 0; line < grid; line++) {
        float x1 = -dimension2 + line * div_side;
        float x2 = x1 + div_side;

        float t_x1 = (line * div_side) / length;
        float t_x2 = ((line + 1) * div_side) / length;

        for (int collumn = 0; collumn < grid; collumn++) {
            float y1 = -dimension2 + collumn * div_side;
            float y2 = y1 + div_side;

            float t_y1 = (collumn * div_side) / length;
            float t_y2 = ((collumn + 1) * div_side) / length;

            // Triangle 1
            add_point_full(Point(x1, y1, -dimension2), normal, Point(t_x1, t_y1, 0.0f));
            add_point_full(Point(x1, y2, -dimension2), normal, Point(t_x1, t_y2, 0.0f));
            add_point_full(Point(x2, y1, -dimension2), normal, Point(t_x2, t_y1, 0.0f));

            // Triangle 2
            add_point_full(Point(x2, y1, -dimension2), normal, Point(t_x2, t_y1, 0.0f));
            add_point_full(Point(x1, y2, -dimension2), normal, Point(t_x1, t_y2, 0.0f));
            add_point_full(Point(x2, y2, -dimension2), normal, Point(t_x2, t_y2, 0.0f));
        }
    }

    normal = Point(-1.0f, 0.0f, 0.0f);
    // Left face
    for (int line = 0; line < grid; line++) {
        float z1 = -dimension2 + line * div_side;
        float z2 = z1 + div_side;

        float t_z1 = (line * div_side) / length;
        float t_z2 = ((line + 1) * div_side) / length;

        for (int collumn = 0; collumn < grid; collumn++) {
            float y1 = -dimension2 + collumn * div_side;
            float y2 = y1 + div_side;

            float t_y1 = (collumn * div_side) / length;
            float t_y2 = ((collumn + 1) * div_side) / length;

            // Triangle 1
            add_point_full(Point(-dimension2, z1, y1), normal, Point(t_z1, t_y1, 0.0f));
            add_point_full(Point(-dimension2, z2, y2), normal, Point(t_z2, t_y2, 0.0f));
            add_point_full(Point(-dimension2, z2, y1), normal, Point(t_z2, t_y1, 0.0f));

            // Triangle 2
            add_point_full(Point(-dimension2, z2, y2), normal, Point(t_z2, t_y2, 0.0f));
            add_point_full(Point(-dimension2, z1, y1), normal, Point(t_z1, t_y1, 0.0f));
            add_point_full(Point(-dimension2, z1, y2), normal, Point(t_z1, t_y2, 0.0f));
        }
    }

    normal = Point(1.0f, 0.0f, 0.0f);
    // Right face
    for (int line = 0; line < grid; line++) {
        float z1 = -dimension2  + line * div_side;
        float z2 = z1 + div_side;

        float t_z1 = (line * div_side) / length;
        float t_z2 = ((line + 1) * div_side) / length;

        for (int collumn = 0; collumn < grid; collumn++) {
            float y1 = -dimension2  + collumn * div_side;
            float y2 = y1 + div_side;

            float t_y1 = (collumn * div_side) / length;
            float t_y2 = ((collumn + 1) * div_side) / length;

            // Triangle 1
            add_point_full(Point(dimension2, z1, y1), normal, Point(t_z1, t_y1, 0.0f));
            add_point_full(Point(dimension2, z2, y1), normal, Point(t_z2, t_y1, 0.0f));
            add_point_full(Point(dimension2, z2, y2), normal, Point(t_z2, t_y2, 0.0f));


            // Triangle 2
            add_point_full(Point(dimension2, z2, y2), normal, Point(t_z2, t_y2, 0.0f));
            add_point_full(Point(dimension2, z1, y2), normal, Point(t_z1, t_y2, 0.0f));
            add_point_full(Point(dimension2, z1, y1), normal, Point(t_z1, t_y1, 0.0f));
        }
    }
}

Figure::FigureType Box::get_type() {
	return Figure::FigureType::BOX;
}