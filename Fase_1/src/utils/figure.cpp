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

Figure::Figure() {}
Figure::Figure(const std::vector<Point>& points) : points(points) {}
Figure::~Figure() { points.clear(); }

void Figure::add_point(const Point& point) {
    points.push_back(point);
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

    for (const auto& point : points) {
        file << point.x << ';' << point.y << ';' << point.z << '\n';
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

    while (std::getline(file, line)) {
        std::vector<std::string> tokens = split(line, ';');
        Point point(std::stof(tokens.at(0)), std::stof(tokens.at(1)), std::stof(tokens.at(2)));
        points.push_back(point);
    }

    std::vector<int> args;
    for (unsigned int i = 2; i < first_line.size(); i++)
        args.push_back(std::stoi(first_line.at(i)));

    file.close();
    Figure* instance = nullptr;

    if (type == FigureType::BOX && args.size() >= 2) {
        instance = new Box(args.at(0), args.at(1), points);
    } else if (type == FigureType::CONE && args.size() >= 4) {
        instance = new Cone(args.at(0), args.at(1), args.at(2), args.at(3), points);
    } else if (type == FigureType::PLANE && args.size() >= 2) {
        instance = new Plane(args.at(0), args.at(1), 0, points);
    } else if (type == FigureType::SPHERE && args.size() >= 3) {
        instance = new Sphere(args.at(0), args.at(1), args.at(2), points);
    } else {
        std::cerr << "Incorrect number of arguments for type T." << std::endl;
    }

    return instance;
}