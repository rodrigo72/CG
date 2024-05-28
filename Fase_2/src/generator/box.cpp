#include "box.hpp"
#include "plane.hpp"

Box::Box(int length, int grid, const std::vector<Point>& points) : length(length), grid(grid), Figure(points) {}
Box::~Box() {}

void Box::generate_points() {
	points.clear();

    float dimension2 = static_cast<float>(length) / 2;
    float div_side = static_cast<float>(length) / grid;

    // Bottom face (reversed plane)
    for (int line = 0; line < grid; line++) {
        float z1 = -dimension2 + line * div_side;
        float z2 = z1 + div_side;
        for (int column = 0; column < grid; column++) {
            float x1 = -dimension2 + column * div_side;
            float x2 = x1 + div_side;

            // 1 -- 3
            // | \  |
            // |  \ |
            // 2 -- 4

            // First triangle
            add_point(Point(x1, -dimension2, z1)); // 1
            add_point(Point(x2, -dimension2, z1)); // 3
            add_point(Point(x2, -dimension2, z2)); // 2

            // Second triangle
            add_point(Point(x2, -dimension2, z2)); // 3
            add_point(Point(x1, -dimension2, z2)); // 4
            add_point(Point(x1, -dimension2, z1)); // 2

        }
    }

    // Top face (normal plane)
    for (int line = 0; line < grid; line++) {
        float z1 = -dimension2 + line * div_side;
        float z2 = z1 + div_side;
        for (int column = 0; column < grid; column++) {
            float x1 = -dimension2 + column * div_side;
            float x2 = x1 + div_side;

            // 1 -- 3
            // | \  |
            // |  \ |
            // 2 -- 4

            // First triangle
            add_point(Point(x1, dimension2, z1)); // 1
            add_point(Point(x1, dimension2, z2)); // 2
            add_point(Point(x2, dimension2, z2)); // 4

            // Second triangle
            add_point(Point(x2, dimension2, z2)); // 4
            add_point(Point(x2, dimension2, z1)); // 3
            add_point(Point(x1, dimension2, z1)); // 1
        }
    }

    // Front face
    for (int line = 0; line < grid; line++) {
        float x1 = -dimension2  + line * div_side;
        float x2 = x1 + div_side;
        for (int collumn = 0; collumn < grid; collumn++) {
            float y1 = -dimension2  + collumn * div_side;
            float y2 = y1 + div_side;

            // Triangle 1
            add_point(Point(x1, y1, dimension2));
            add_point(Point(x2, y1, dimension2));
            add_point(Point(x1, y2, dimension2));

            // Triangle 2
            add_point(Point(x2, y2, dimension2));
            add_point(Point(x1, y2, dimension2));
            add_point(Point(x2, y1, dimension2));
        }
    }

    // Back face
    for (int line = 0; line < grid; line++) {
        float x1 = -dimension2 + line * div_side;
        float x2 = x1 + div_side;
        for (int collumn = 0; collumn < grid; collumn++) {
            float y1 = -dimension2 + collumn * div_side;
            float y2 = y1 + div_side;

            // Triangle 1
            add_point(Point(x1, y1, -dimension2));
            add_point(Point(x1, y2, -dimension2));
            add_point(Point(x2, y1, -dimension2));

            // Triangle 2
            add_point(Point(x2, y1, -dimension2));
            add_point(Point(x1, y2, -dimension2));
            add_point(Point(x2, y2, -dimension2));
        }
    }

    // Left face
    for (int line = 0; line < grid; line++) {
        float z1 = -dimension2 + line * div_side;
        float z2 = z1 + div_side;
        for (int collumn = 0; collumn < grid; collumn++) {
            float y1 = -dimension2 + collumn * div_side;
            float y2 = y1 + div_side;

            // Triangle 1
            add_point(Point(-dimension2, z1, y1));
            add_point(Point(-dimension2, z2, y2));
            add_point(Point(-dimension2, z2, y1));

            // Triangle 2
            add_point(Point(-dimension2, z2, y2));
            add_point(Point(-dimension2, z1, y1));
            add_point(Point(-dimension2, z1, y2));
        }
    }

    // Right face
    for (int line = 0; line < grid; line++) {
        float z1 = -dimension2  + line * div_side;
        float z2 = z1 + div_side;
        for (int collumn = 0; collumn < grid; collumn++) {
            float y1 = -dimension2  + collumn * div_side;
            float y2 = y1 + div_side;

            // Triangle 1
            add_point(Point(dimension2, z1, y1));
            add_point(Point(dimension2, z2, y1));
            add_point(Point(dimension2, z2, y2));


            // Triangle 2
            add_point(Point(dimension2, z2, y2));
            add_point(Point(dimension2, z1, y2));
            add_point(Point(dimension2, z1, y1));
        }
    }
}

Figure::FigureType Box::get_type() {
	return Figure::FigureType::BOX;
}