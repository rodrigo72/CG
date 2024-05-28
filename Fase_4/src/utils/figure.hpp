#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <string>
#include <cmath>
constexpr float PI = 3.14159265358979323846f;

#include <vector>
#include "point.hpp"

class Figure {
public:
    Figure(
        const std::vector<Point>& points = {},
        const std::vector<Point>& normals = {}, 
        const std::vector<Point>& texture_coords = {}
    );
    ~Figure();

    std::vector<Point> points;
    std::vector<Point> normals;
    std::vector<Point> texture_coords;

    std::string texture_file;

    std::string to_string() const;
    vector<float> to_vector();
    vector<float> get_normals_vector();
    vector<float> get_texture_coords_vector();

    vector<float> get_diffuse();
    void set_diffuse(float r, float g, float b);

    vector<float> get_ambient();
    void set_ambient(float r, float g, float b);

    vector<float> get_specular();
    void set_specular(float r, float g, float b);

    vector<float> get_emissive();
    void set_emissive(float r, float g, float b);

    float get_shininess();
    void set_shininess(float shininess);

    enum FigureType { FIGURE = 0, BOX = 1, CONE = 2, PLANE = 3, SPHERE = 4, RING = 5, BEZIER = 6 };

    void add_point(const Point& point);
    void add_point_full(const Point& point, Point& normal, const Point& texture_coord);
    void add_point_full(const Point& point, Point& normal);

    void to_file(const std::string& path, const std::vector<int>& args, FigureType type);
    static Figure* from_file(const std::string& path);
    
    virtual void generate_points() = 0;    
    virtual FigureType get_type() { return FigureType::FIGURE; };
    virtual std::string get_type_as_string() { return "FIGURE"; };

private:
    vector<float>* diffuse;
    vector<float>* ambient;
    vector<float>* specular;
    vector<float>* emissive;
    float shininess;
};

#endif // FIGURE_HPP
