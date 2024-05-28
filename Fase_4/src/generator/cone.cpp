#include "cone.hpp"
#include <iostream>

Cone::Cone(
    int radius, 
    int height, 
    int slices, 
    int stacks, 
    const std::vector<Point>& points, 
    const std::vector<Point>& normals, 
    const std::vector<Point>& texture_coords
) : radius(radius), height(height), slices(slices), stacks(stacks), Figure(points, normals, texture_coords) {}

Cone::~Cone() {}

void Cone::generate_points() {
	points.clear();

    // Base
    float alpha = (2.0f * PI) / slices;
    float start = 0.0f;
    std::vector<Point> base(slices);
    std::vector<Point> temp_normals(slices);

    Point origin = Point(0.0f, 0.0f, 0.0f);
    Point apex = Point(0.0f, static_cast<float>(height), 0.0f);

    Point normal = Point(0.0f, -1.0f, 0.0f);

    float raio_tex = 0.5f;
    float center_tex[2] = { 0.5f, 0.5f };

    for (int i = 0; i < slices; i++) {
        Point np = Point(radius * sinf(start), 0.0f, radius * cosf(start));
        base.insert(base.begin() + i, np);

        add_point_full(np, normal, Point::new_sph_point2(center_tex, start, raio_tex));
        add_point_full(Point(0.0f, 0.0f, 0.0f), normal, Point(0.5f, 0.5f, 0.0f));
        start += alpha;
        add_point_full(Point(radius * sinf(start), 0.0f, radius * cosf(start)), normal, Point::new_sph_point2(center_tex, start, raio_tex));

        // normals
        Point vector_a = Point::vector_from_two_points(origin, np);
        Point vector_b = Point::vector_from_two_points(origin, apex);
        Point vector_c = Point::vector_from_two_points(np, apex);
        Point vector_r = Point::cross2(vector_a, vector_b);

        Point edge_normal = Point::cross2(vector_c, vector_r);
        edge_normal.normalize();

        auto position = temp_normals.begin() + i;
        temp_normals.insert(position, edge_normal);
    }

    float h_diff = static_cast<float>(height) / stacks;

    // Faces
    for (int j = 0; j < slices; j++) {

        float fj = static_cast<float>(j);

        Point p1 = base.at(j);
        Point normal1 = temp_normals.at(j);
        int nj = j + 1;

        if (nj == slices) 
            nj = 0;
        Point p2 = base.at(nj);
        Point normal2 = temp_normals.at(nj);
        
        float current_texture_radius = 0.5f;
 
        float b1x_diff = p1.x / stacks; // Diferença entre a coordenada X e 0 do p1
        float b1z_diff = p1.z / stacks; // Diferença entre a coordenada Z e 0 do p1

        float b2x_diff = p2.x / stacks; // Diferença entre a coordenada X e 0 do p2
        float b2z_diff = p2.z / stacks; // Diferença entre a coordenada Z e 0 do p2

        Point l_p1 = p1;
        Point l_p2 = p2;

        Point t1, t2, t3, t4;

        // Para cada stack da face
        for (int i = 0; i < stacks - 1; i++) {
            // Triangulo da esquerda
            // 1
            // 1  2

            float fi = static_cast<float>(i);
            //float texture_step = (0.5f / slices) / stacks;

            // 3  4
            // 1  2
            t1 = Point(
                fj / slices, 
                fi / stacks, 
                0.0f
            );
            t2 = Point(
                (fj + 1) / slices,
                fi / stacks, 
                0.0f
            );
            t3 = Point(
                fj / slices, 
                (fi + 1) / stacks, 
                0.0f
            );
            t4 = Point(
                (fj + 1) / slices,
                (fi + 1) / stacks, 
                0.0f
            );

            add_point_full(l_p1, normal1, t1);
            add_point_full(l_p2, normal2, t2);
            l_p1 = Point(l_p1.x - b1x_diff, l_p1.y + h_diff, l_p1.z - b1z_diff);
            add_point_full(l_p1, normal1, t3);

            // Triangulo da direita
            // 1  2
            //    2
            add_point_full(l_p2, normal2, t2);
            l_p2 = Point(l_p2.x - b2x_diff, l_p2.y + h_diff, l_p2.z - b2z_diff);
            add_point_full(l_p2, normal2, t4);
            add_point_full(l_p1, normal1, t3);
        }

        Point t5 = Point(
            (fj + 0.5f) / slices,
            1.0f,
            0.0f
        );

        // Construir triangulo do topo.
        add_point_full(l_p1, normal1, t3);
        add_point_full(l_p2, normal2, t4);
        add_point_full(Point(0.0f, static_cast<float>(height), 0.0f), t5);
    }

    base.clear();
    temp_normals.clear();

}

Figure::FigureType Cone::get_type() {
	return Figure::FigureType::CONE;
}