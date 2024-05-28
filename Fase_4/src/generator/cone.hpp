#ifndef CONE_HPP
#define CONE_HPP

#include "../utils/figure.hpp"

class Cone : public Figure {
public:
    Cone(
        int radius, 
        int height, 
        int slices, 
        int stacks, 
        const std::vector<Point>& points = {},
        const std::vector<Point>& normals = {}, 
        const std::vector<Point>& texture_coords = {}
    );

    ~Cone();

    int radius;
    int height;
    int slices;
    int stacks;

    virtual void generate_points() override;
    FigureType get_type() override;
    std::string get_type_as_string() override {
        return "CONE";
    };
};

#endif // CONE_HPP
