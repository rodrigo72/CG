#ifndef Ring_HPP
#define Ring_HPP

#include "../utils/figure.hpp"
#include "../utils/point.hpp"

class Ring : public Figure {
public:
    Ring(
        int iRadius, 
        int eRadius, 
        int slices, 
        const std::vector<Point>& points = {}, 
        const std::vector<Point>& normals = {}, 
        const std::vector<Point>& texture_coords = {}
    );

    ~Ring();

    int iRadius;
    int eRadius;
    int slices;

    virtual void generate_points() override;
    FigureType get_type() override;
    std::string get_type_as_string() override {
        return "RING";
    };
};

#endif // Ring_HPP
