#ifndef CONE_HPP
#define CONE_HPP

#include "../utils/figure.hpp"

class Cone : public Figure {
public:
    Cone(int radius, int height, int slices, int stacks, const std::vector<Point>& points = {});

    ~Cone();

    int radius;
    int height;
    int slices;
    int stacks;

    virtual void generate_points() override;
    FigureType get_type() override;
};

#endif // CONE_HPP
