#include "cone.hpp"

Cone::Cone(int radius, int height, int slices, int stacks, const std::vector<Point>& points) : radius(radius), height(height), slices(slices), stacks(stacks), Figure(points) {}
Cone::~Cone() {}

void Cone::generate_points() {
	points.clear();

    // Base
    float alpha = (2.0f * PI) / slices;
    float start = 0.0f;
    std::vector<Point> base(slices);

    for (int i = 0; i < slices; i++) {
        Point np = Point(radius * sinf(start), 0.0f, radius * cosf(start));
        base.insert(base.begin() + i, np);

        add_point(np);
        add_point(Point(0.0f, 0.0f, 0.0f));
        start += alpha;
        add_point(Point(radius * sinf(start), 0.0f, radius * cosf(start)));
    }

    float h_diff = static_cast<float>(height) / stacks;

    // Faces
    for (int j = 0; j < slices; j++) {
        Point p1 = base.at(j);
        int nj = j + 1;

        if (nj == slices) 
            nj = 0;
        Point p2 = base.at(nj);

        float b1x_diff = p1.x / stacks; // Diferença entre a coordenada X e 0 do p1
        float b1z_diff = p1.z / stacks; // Diferença entre a coordenada Z e 0 do p1

        float b2x_diff = p2.x / stacks; // Diferença entre a coordenada X e 0 do p2
        float b2z_diff = p2.z / stacks; // Diferença entre a coordenada Z e 0 do p2

        Point l_p1 = p1;
        Point l_p2 = p2;

        // Para cada stack da face
        for (int i = 0; i < stacks - 1; i++) {
            // Triangulo da esquerda
            add_point(l_p1);
            add_point(l_p2);
            l_p1 = Point(l_p1.x - b1x_diff, l_p1.y + h_diff, l_p1.z - b1z_diff);
            add_point(l_p1);

            // Triangulo da direita
            add_point(l_p2);
            l_p2 = Point(l_p2.x - b2x_diff, l_p2.y + h_diff, l_p2.z - b2z_diff);
            add_point(l_p2);
            add_point(l_p1);
        }

        // Construir triangulo do topo.
        add_point(l_p1);
        add_point(l_p2);
        add_point(Point(0.0f, static_cast<float>(height), 0.0f));
    }

}

Figure::FigureType Cone::get_type() {
	return Figure::FigureType::CONE;
}