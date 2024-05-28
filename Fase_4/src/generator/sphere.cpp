#include "sphere.hpp"
#include <iostream>

Sphere::Sphere(
    int radius,
    int slices, 
    int stacks, 
    const std::vector<Point>& points, 
    const std::vector<Point>& normals,
    const std::vector<Point>& texture_coords
) : radius(radius), slices(slices), stacks(stacks), Figure(points, normals, texture_coords) {}

Sphere::~Sphere() {}

void Sphere::generate_points() {
    points.clear();

    float sector_step = (2 * PI) / slices;
    float theta, theta_next; // horizontal

    float stack_step = PI / stacks;
    float phi, phi_next; // vertical

    float xy, y, xy_next, y_next;

    for (int i = 0; i < stacks; i++) {

        phi = (PI / 2) - (i * stack_step); // pi/2 até -pi/2

        /* os pontos numa linha horizontal de um setor têm todos a mesma:
            - altura;
            - distancia da origem até à projeção do ponto no plano XY (componete de cálculo) */
        xy = radius * cosf(phi);
        y = radius * sinf(phi);

        // proxima linha
        phi_next = (PI / 2) - ((i + 1) * stack_step);
        xy_next = radius * cosf(phi_next);
        y_next = radius * sinf(phi_next);

        /* x, e z irão variar de acordo com o theta
        (x e z serão o valor de x e z do ponto projetado
        mas neste nesta parte do código ainda não se sabe qual é esse ponto, pois não se conhece o theta;
        será introduzido no loop seguinte) */

        for (int j = 0; j < slices; j++) {

            // 0 até 2pi
            theta = j * sector_step;
            theta_next = (j + 1) * sector_step;

            //  p1--p4
            //  | / |
            //  p2--p3

            Point p1 = Point(xy      * sinf(theta),      y,      xy      * cosf(theta) ); // ((r * cos(phi)) * cos(theta), r * sin(phi), (r * cos(phi)) * sin(theta))
            Point p2 = Point(xy_next * sinf(theta),      y_next, xy_next * cosf(theta) ); // phi move
            Point p3 = Point(xy_next * sinf(theta_next), y_next, xy_next * cosf(theta_next) ); // phi e theta movem
            Point p4 = Point(xy      * sinf(theta_next), y,      xy      * cosf(theta_next) ); // theta move

            Point p1_normal = Point::normalize2(p1);
            Point p2_normal = Point::normalize2(p2);
            Point p3_normal = Point::normalize2(p3);
            Point p4_normal = Point::normalize2(p4);

            Point t1 = Point(
                static_cast<float>(j) / slices,
                static_cast<float>(stacks - i) / stacks,
                0.0f
            );
            Point t2 = Point(
                static_cast<float>(j) / slices,
                static_cast<float>(stacks - i  - 1) / stacks,
                0.0f
            );
            Point t3 = Point(
                static_cast<float>(j + 1) / slices,
                static_cast<float>(stacks - i - 1) / stacks,
                0.0f
            );
            Point t4 = Point(
                static_cast<float>(j + 1) / slices,
                static_cast<float>(stacks - i) / stacks,
                0.0f
            );
            
            // criar uma "fila" de triangulos com a forma de um paralelogramo:

            // primeiro triangulo
            if (i != 0) {
                add_point_full(p1, p1_normal, t1);
                add_point_full(p2, p2_normal, t2);
                add_point_full(p4, p4_normal, t4);
            }

            // segundo triangulo
            if (i != stacks - 1) {
                add_point_full(p2, p2_normal, t2);
                add_point_full(p3, p3_normal, t3);
                add_point_full(p4, p4_normal, t4);
            }

        }
    }

}

Figure::FigureType Sphere::get_type() {
    return Figure::FigureType::SPHERE;
}