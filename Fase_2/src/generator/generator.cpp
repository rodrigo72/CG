#include "sphere.hpp"
#include "plane.hpp"
#include "box.hpp"
#include "cone.hpp"
#include "ring.hpp"

#include "../utils/figure.hpp"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;


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
        else {
            printf("Invalid\n");
            return 1;
        }

        figure->generate_points();
        figure->to_file(file_path, args, figure->get_type());

        /*Figure* f = Figure::from_file(file_path);

        if (f->get_type() == Figure::FigureType::SPHERE) {
            std::cout << "Sphere!" << std::endl;
            Sphere* sphere = dynamic_cast<Sphere*>(f);
        }*/

        args.clear();
        delete figure;
    }
    else{
        printf("Invalid number of arguments.\n");
        return 1;
    }
    return 0;
}
