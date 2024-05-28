#include "sphere.hpp"

Sphere::Sphere(int radius, int slices, int stacks, const std::vector<Point>& points) : radius(radius), slices(slices), stacks(stacks), Figure(points) {}
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
            //  | \ |
            //  p2--p3

            Point p1 = Point(xy      * sinf(theta),      y,      xy      * cosf(theta) ); // ((r * cos(phi)) * cos(theta), r * sin(phi), (r * cos(phi)) * sin(theta))
            Point p2 = Point(xy_next * sinf(theta),      y_next, xy_next * cosf(theta) ); // phi move
            Point p3 = Point(xy_next * sinf(theta_next), y_next, xy_next * cosf(theta_next) ); // phi e theta movem
            Point p4 = Point(xy      * sinf(theta_next), y,      xy      * cosf(theta_next) ); // theta move

            // criar uma "fila" de triangulos com a forma de um paralelogramo:

            // primeiro triangulo
            if (i != stacks - 1) {
                add_point(p1);
                add_point(p2);
                add_point(p3);
            }

            // segundo triangulo
            if (i != 0) {
                add_point(p3);
                add_point(p4);
                add_point(p1);
            }

        }
    }
}

Figure::FigureType Sphere::get_type() {
    return Figure::FigureType::SPHERE;
}