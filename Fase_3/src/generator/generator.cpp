#include "sphere.hpp"
#include "plane.hpp"
#include "box.hpp"
#include "cone.hpp"
#include "ring.hpp"
#include "bezier.hpp"

#include "../utils/figure.hpp"
#include "../utils/matrix.hpp"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

vector<vector<Point>> read_patches_file(const char* file_path) {
    ifstream input_file(file_path);

    if (!input_file.is_open()) {
        cerr << "[read_patches_file] Error opening file" << endl;
        return vector<vector<Point>>();
    }

    string line;
    if (!getline(input_file, line)) {
        cerr << "[read_patches_file] Error reading line" << endl;
        return vector<vector<Point>>();
    }

    int num_patches;
    istringstream(line) >> num_patches;

    vector<vector<int>> idxs_per_patch;
    for (int i = 0; i < num_patches && getline(input_file, line); i++) {
        std::istringstream iss(line);
        vector<int> idxs;
        int value;
        char comma;

        while (iss >> value >> comma) idxs.push_back(value);
        if (iss.eof()) idxs.push_back(value);

        idxs_per_patch.push_back(idxs);
    }

    if (!getline(input_file, line)) {
        cerr << "[read_patches_file] Error reading line" << endl;
        return vector<vector<Point>>();
    }

    int num_pontos;
    istringstream(line) >> num_pontos;

    vector<Point> control_points;
    for (int i = 0; i < num_pontos && getline(input_file, line); i++) {
        std::istringstream iss(line);
        float x, y, z;
        char comma;
        if (!(iss >> x >> comma >> y  >> comma >> z)) {
            cerr << "[read_patches_file] Error reading control point at line " << i + 1 << endl;
            return vector<vector<Point>>();
        }
        control_points.push_back(Point(x, y, z));
    }

    vector<vector<Point>> result;
    for (vector<int> idxs : idxs_per_patch) {
        vector<Point> patch;
        for (int idx : idxs) {
            patch.push_back(Point(control_points[idx]));
        }
        result.push_back(patch);
    }

    input_file.close();
    return result;
}

int main(int argc, char *argv[]){
    if (argc >= 5){
        Figure* figure;
        const char *file_path;
        vector<int> args;

        if (strcmp(argv[1], "plane") == 0) {
            int length = atoi(argv[2]), divisions = atoi(argv[3]); 
            file_path = argv[4];

            args.insert(args.end(), { length, divisions });
            figure = new Plane(length, divisions);
        }
        else if (strcmp(argv[1], "box") == 0) {
            int length = atoi(argv[2]), grid = atoi(argv[3]);
            file_path = argv[4];

            args.insert(args.end(), { length, grid });
            figure = new Box(length, grid);
        }
        else if (strcmp(argv[1], "sphere") == 0) {
            int radius = atoi(argv[2]), slices = atoi(argv[3]), stacks = atoi(argv[4]);
            file_path = argv[5];

            args.insert(args.end(), { radius, slices, stacks });
            figure = new Sphere(radius, slices, stacks); 
        }
        else if (strcmp(argv[1], "cone") == 0) {
            int radius = atoi(argv[2]), height = atoi(argv[3]), slices = atoi(argv[4]), stacks = atoi(argv[5]);
            file_path = argv[6];

            args.insert(args.end(), { radius, height, slices, stacks });
            figure = new Cone(radius, height, slices, stacks);
        }
        else if (strcmp(argv[1], "ring") == 0) {
            int radius = atoi(argv[2]), radius2 = atoi(argv[3]), slices = atoi(argv[4]);
            file_path = argv[5];

            args.insert(args.end(), { radius, radius2, slices});
            figure = new Ring(radius, radius2, slices);
        }
        else if (strcmp(argv[1], "patch") == 0) {
            int tessellation = atoi(argv[2]);
            const char* patches_file_path = argv[3];
            file_path = argv[4];

            args.insert(args.end(), { tessellation });
            vector<vector<Point>> points_per_patch = read_patches_file(patches_file_path);
            figure = new Bezier(tessellation, points_per_patch);
        }
        else {
            printf("Invalid\n");
            return 1;
        }

        figure->generate_points();
        figure->to_file(file_path, args, figure->get_type());

        args.clear();
        delete figure;
    }
    else{
        printf("Invalid number of arguments.\n");
        return 1;
    }
    return 0;
}
