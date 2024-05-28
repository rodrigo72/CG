#include "ring.hpp"

Ring::Ring(int iRadius, int eRadius, int slices, const std::vector<Point>& points) : iRadius(iRadius), eRadius(eRadius), slices(slices), Figure(points) {}
Ring::~Ring() {}

void Ring::generate_points() {
    points.clear();
    float a = 0, delta = (2 * PI) / slices;

    float f_iRadius = static_cast<float>(iRadius);
    float f_eRadius = static_cast<float>(eRadius);


    for (int i = 0; i < slices; i++, a += delta) {
            add_point(Point::new_sph_point(a, 0.0f, f_iRadius));
            add_point(Point::new_sph_point(a, 0.0f, f_eRadius));
            add_point(Point::new_sph_point(a + delta, 0.0f, f_iRadius));

            add_point(Point::new_sph_point(a + delta, 0.0f, f_iRadius));
            add_point(Point::new_sph_point(a, 0.0f, f_eRadius));
            add_point(Point::new_sph_point(a + delta, 0.0f, f_eRadius));

            add_point(Point::new_sph_point(a + delta, 0.0f, f_iRadius));
            add_point(Point::new_sph_point(a, 0.0f, f_eRadius));
            add_point(Point::new_sph_point(a, 0.0f, f_iRadius));

            add_point(Point::new_sph_point(a, 0.0f, f_eRadius));
            add_point(Point::new_sph_point(a + delta, 0.0f, f_iRadius));
            add_point(Point::new_sph_point(a + delta, 0.0f, f_eRadius));
    }
}

Figure::FigureType Ring::get_type() {
    return Figure::FigureType::RING;
}