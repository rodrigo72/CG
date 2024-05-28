#ifndef Ring_HPP
#define Ring_HPP

#include "../utils/figure.hpp"
#include "../utils/point.hpp"

class Ring : public Figure {
public:
    Ring(int iRadius, int eRadius, int slices, const std::vector<Point>& points = {});

    ~Ring();

    int iRadius;
    int eRadius;
    int slices;

    virtual void generate_points() override;
    FigureType get_type() override;
};

#endif // Ring_HPP
