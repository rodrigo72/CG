#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <stdexcept>

#include <map>
#include "figure.hpp"
#include "../generator/box.hpp"
#include "../generator/cone.hpp"
#include "../generator/plane.hpp"
#include "../generator/sphere.hpp"
#include "../generator/ring.hpp"
#include "../generator/bezier.hpp"

Figure::Figure(
    const std::vector<Point>& points, 
    const std::vector<Point>& normals, 
    const std::vector<Point>& texture_coords
) : points(points), normals(normals), texture_coords(texture_coords) {

    this->diffuse = new vector<float>();
    this->ambient = new vector<float>();
    this->specular = new vector<float>();
    this->emissive = new vector<float>();

    this->diffuse->push_back(200.0f); this->diffuse->push_back(200.0f); this->diffuse->push_back(200.0f); this->diffuse->push_back(1.0f);
    this->ambient->push_back(50.0f);  this->ambient->push_back(50.0f);  this->ambient->push_back(50.0f);  this->ambient->push_back(1.0f);
    this->specular->push_back(0.0f);  this->specular->push_back(0.0f);  this->specular->push_back(0.0f);  this->specular->push_back(1.0f);
    this->emissive->push_back(0.0f);  this->emissive->push_back(0.0f);  this->emissive->push_back(0.0f);  this->emissive->push_back(1.0f);
    this->shininess = 0.0f;
}

Figure::~Figure() { 
    points.clear(); 
    normals.clear();
    texture_coords.clear();
    diffuse->clear();
    ambient->clear();
    specular->clear();
    emissive->clear();
    delete diffuse;
    delete ambient;
    delete specular;
    delete emissive;
}

// fase 1, 2 e 3
void Figure::add_point(const Point& point) {
    points.push_back(point);
    normals.push_back(Point(0.0f, 0.0f, 0.0f));
    texture_coords.push_back(Point(0.0f, 0.0f, 0.0f));
}

// fase 4
void Figure::add_point_full(const Point& point, Point& normal, const Point& texture_coord) {
    points.push_back(point);
    normal.normalize();
    normals.push_back(normal);
    texture_coords.push_back(texture_coord);
}

void Figure::add_point_full(const Point& point, Point& normal) {
    points.push_back(point);
    normal.normalize();
    normals.push_back(normal);
    texture_coords.push_back(Point(0.0f, 0.0f, 0.0f));
}

void Figure::to_file(const std::string& path, const std::vector<int>& args, FigureType type) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file '" << path << "'\n";
        return;
    }

    file << static_cast<int>(type) << ';' << points.size();
    for (const auto& arg : args) { file << ';' << arg;  }
    file << '\n';

    for (unsigned int i = 0; i < points.size(); i++) {
        Point p = points.at(i);
        Point n = normals.at(i);
        Point t = texture_coords.at(i);
        file << p.x << ';' << p.y << ';' << p.z << ';';
        file << n.x << ';' << n.y << ';' << n.z << ';';
        file << t.x << ';' << t.y << '\n';
    }

    file.close();
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

Figure* Figure::from_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file '" << path << "'" << std::endl;
        return nullptr;
    }

    std::string line;

    std::getline(file, line);
    std::vector<std::string> first_line = split(line, ';');

    FigureType type = static_cast<FigureType>(std::stoi(first_line.at(0)));
    int n_vertices = std::stoi(first_line.at(1));
    std::vector<Point> points(n_vertices);
    std::vector<Point> normals(n_vertices);
    std::vector<Point> texture_coords(n_vertices);

    while (std::getline(file, line)) {
        std::vector<std::string> tokens = split(line, ';');
        Point p(std::stof(tokens.at(0)), std::stof(tokens.at(1)), std::stof(tokens.at(2)));
        Point n(std::stof(tokens.at(3)), std::stof(tokens.at(4)), std::stof(tokens.at(5)));
        Point t(std::stof(tokens.at(6)), std::stof(tokens.at(7)), 0.0f);
        points.push_back(p);
        normals.push_back(n);
        texture_coords.push_back(t);
    }

    std::vector<int> args;
    for (unsigned int i = 2; i < first_line.size(); i++)
        args.push_back(std::stoi(first_line.at(i)));

    file.close();
    Figure* instance = nullptr;

    if (type == FigureType::BOX && args.size() >= 2) {
        instance = new Box(args.at(0), args.at(1), points, normals, texture_coords);
    } else if (type == FigureType::CONE && args.size() >= 4) {
        instance = new Cone(args.at(0), args.at(1), args.at(2), args.at(3), points, normals, texture_coords);
    } else if (type == FigureType::PLANE && args.size() >= 2) {
        instance = new Plane(args.at(0), args.at(1), 0, points, normals, texture_coords);
    } else if (type == FigureType::SPHERE && args.size() >= 3) {
        instance = new Sphere(args.at(0), args.at(1), args.at(2), points, normals, texture_coords);
    } else if (type == FigureType::RING && args.size() >= 3) {
        instance = new Ring(args.at(0), args.at(1), args.at(2), points, normals, texture_coords);
    } else if (type == FigureType::BEZIER && args.size() >= 1) {
        instance = new Bezier(args.at(0), {}, points, normals, texture_coords);
    } else {
        std::cerr << "Incorrect number of arguments for type T." << std::endl;
        return nullptr;
    }

    return instance;
}

vector<float> Figure::to_vector() {
    vector<float> result;
    for (Point p : this->points) {
        result.push_back(p.x);
        result.push_back(p.y);
        result.push_back(p.z);
    }
    return result;
}

vector<float> Figure::get_normals_vector() {
    vector<float> result;
    for (Point p : this->normals) {
        result.push_back(p.x);
        result.push_back(p.y);
        result.push_back(p.z);
    }
    return result;
}

vector<float> Figure::get_texture_coords_vector() {
    vector<float> result;
    for (Point p : this->texture_coords) {
        result.push_back(p.x);
        result.push_back(p.y);
    }
    return result;
}

void Figure::set_diffuse(float r, float g, float b) {
    float* aux = this->diffuse->data();
    aux[0] = r;
    aux[1] = g;
    aux[2] = b;
}

vector<float> Figure::get_diffuse() {
    vector<float> result;
    result.push_back(this->diffuse->at(0) / 255.0f);
    result.push_back(this->diffuse->at(1) / 255.0f);
    result.push_back(this->diffuse->at(2) / 255.0f);
    result.push_back(this->diffuse->at(3) / 255.0f);
    return result;
}

void Figure::set_ambient(float r, float g, float b) {
    float* aux = this->ambient->data();
    aux[0] = r;
    aux[1] = g;
    aux[2] = b;
}

vector<float> Figure::get_ambient() {
    vector<float> result;
    result.push_back(this->ambient->at(0) / 255.0f);
    result.push_back(this->ambient->at(1) / 255.0f);
    result.push_back(this->ambient->at(2) / 255.0f);
    result.push_back(this->ambient->at(3) / 255.0f);
    return result;
}

void Figure::set_specular(float r, float g, float b) {
    float* aux = this->specular->data();
    aux[0] = r;
    aux[1] = g;
    aux[2] = b;
}

vector<float> Figure::get_specular() {
    vector<float> result;
    result.push_back(this->specular->at(0) / 255.0f);
    result.push_back(this->specular->at(1) / 255.0f);
    result.push_back(this->specular->at(2) / 255.0f);
    result.push_back(this->specular->at(3) / 255.0f);
    return result;
}


void Figure::set_emissive(float r, float g, float b) {
    float* aux = this->emissive->data();
    aux[0] = r;
    aux[1] = g;
    aux[2] = b;
}

vector<float> Figure::get_emissive() {
    vector<float> result;
    result.push_back(this->emissive->at(0) / 255.0f);
    result.push_back(this->emissive->at(1) / 255.0f);
    result.push_back(this->emissive->at(2) / 255.0f);
    result.push_back(this->emissive->at(3) / 255.0f);
    return result;
}

void Figure::set_shininess(float shininess) {
    this->shininess = shininess;
}

float Figure::get_shininess() {
    return this->shininess;
}

std::string Figure::to_string() const {
    std::stringstream ss;

    ss << "Texture file: " << texture_file << '\n';
    ss << "Diffuse: " << diffuse->at(0) << ", " << diffuse->at(1) << ", " << diffuse->at(2) << '\n';
    ss << "Ambient: " << ambient->at(0) << ", " << ambient->at(1) << ", " << ambient->at(2) << '\n';
    ss << "Specular: " << specular->at(0) << ", " << specular->at(1) << ", " << specular->at(2) << '\n';
    ss << "Emissive: " << emissive->at(0) << ", " << emissive->at(1) << ", " << emissive->at(2) << '\n';
    ss << "Shininess: " << shininess << '\n';

    /*ss << "Points " << "(" << points.size() << "):" << '\n';
    for (const auto& point : points) {
        ss << "  (" << point.x << ", " << point.y << ", " << point.z << ")\n";
    }

    ss << "Normals " << "(" << normals.size() << "):" << '\n';
    for (const auto& normal : normals) {
        ss << "  (" << normal.x << ", " << normal.y << ", " << normal.z << ")\n";
    }

    ss << "Texture Coordinates " << "(" << texture_coords.size() << "):" << '\n';
    for (const auto& texture_coord : texture_coords) {
        ss << "  (" << texture_coord.x << ", " << texture_coord.y << ", " << texture_coord.z << ")\n";
    }*/

    return ss.str();
}