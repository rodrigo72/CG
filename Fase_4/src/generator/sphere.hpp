#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "../utils/figure.hpp"

class Sphere : public Figure {
public:
    Sphere(
        int radius, 
        int slices, 
        int stacks, 
        const std::vector<Point>& points = {}, 
        const std::vector<Point>& normals = {}, 
        const std::vector<Point>& texture_coords = {}
    );

    ~Sphere();

    int radius;
    int slices;
    int stacks;

    virtual void generate_points() override;
    FigureType get_type() override;
    std::string get_type_as_string() override {
        return "SPHERE";
    };
};

#endif // SPHERE_HPP
