#ifndef BEZIER_HPP
#define BEZIER_HPP

#include "../utils/figure.hpp"

class Bezier : public Figure {
public:
    Bezier(
        int tesselation, 
        const std::vector<vector<Point>>& points_per_patch, 
        const std::vector<Point>& points = {},
        const std::vector<Point>& normals = {}, 
        const std::vector<Point>& texture_coords = {}
    );
    ~Bezier();

    vector<vector<Point>> points_per_patch;
    int tesselation;

    virtual void generate_points() override;
    FigureType get_type() override;
    std::string get_type_as_string() override {
        return "BEZIER";
    };
};

#endif // BEZIER_HPP
