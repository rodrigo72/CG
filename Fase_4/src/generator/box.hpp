#ifndef BOX_HPP
#define BOX_HPP

#include "../utils/figure.hpp"

class Box : public Figure {
public:
    Box(
        int length, 
        int grid, 
        const std::vector<Point>& pointsVector = {}, 
        const std::vector<Point>& normals = {}, 
        const std::vector<Point>& texture_coords = {}
    );

    ~Box();

    int length;
    int grid;

    virtual void generate_points() override;
    FigureType get_type() override;
    std::string get_type_as_string() override {
        return "BOX";
    };
};

#endif // BOX_HPP
