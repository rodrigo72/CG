#ifndef PLANE_HPP
#define PLANE_HPP

#include "../utils/figure.hpp"

class Plane : public Figure {
public:
    Plane(int length, int divisions, int height = 0, const std::vector<Point>& points = {});

    ~Plane();

    int length;
    int divisions;
    int height;

    virtual void generate_points() override;
    FigureType get_type() override;
};

#endif // PLANE_HPP
