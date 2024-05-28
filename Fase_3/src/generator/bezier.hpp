#ifndef BEZIER_HPP
#define BEZIER_HPP

#include "../utils/figure.hpp"

class Bezier : public Figure {
public:
    Bezier(int tesselation, const std::vector<vector<Point>>& points_per_patch, const std::vector<Point>& points = {});
    ~Bezier();

    vector<vector<Point>> points_per_patch;
    int tesselation;

    virtual void generate_points() override;
    FigureType get_type() override;
};

#endif // BEZIER_HPP
