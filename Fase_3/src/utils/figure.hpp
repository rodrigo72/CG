#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <string>
#include <cmath>
constexpr float PI = 3.14159265358979323846f;

#include <vector>
#include "point.hpp"

class Figure {
public:
    Figure();
    Figure(const std::vector<Point>& points);
    ~Figure();

    std::vector<Point> points;
    enum FigureType { FIGURE = 0, BOX = 1, CONE = 2, PLANE = 3, SPHERE = 4, RING = 5, BEZIER = 6 };

    void add_point(const Point& point);
    void to_file(const std::string& path, const std::vector<int>& args, FigureType type);
    vector<float> to_vector();
    static Figure* from_file(const std::string& path);
    
    virtual void generate_points() = 0;    
    virtual FigureType get_type() { return FigureType::FIGURE; }
};

#endif // FIGURE_HPP
