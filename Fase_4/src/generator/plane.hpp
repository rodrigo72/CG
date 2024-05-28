#ifndef PLANE_HPP
#define PLANE_HPP

#include "../utils/figure.hpp"

class Plane : public Figure {
public:
    Plane(
        int length, 
        int divisions, 
        int height = 0, 
        const std::vector<Point>& points = {}, 
        const std::vector<Point>& normals = {}, 
        const std::vector<Point>& texture_points = {}
    );

    ~Plane();

    int length;
    int divisions;
    int height;

    virtual void generate_points() override;
    FigureType get_type() override;
    std::string get_type_as_string() override {
        return "PLANE";
    };
};

#endif // PLANE_HPP
