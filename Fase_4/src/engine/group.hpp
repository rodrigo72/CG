#ifndef GROUP_HPP
#define GROUP_HPP

#include <vector>
#include <string>
#include "transform.hpp"
#include "group.hpp"

class ModelInfo;
class Transform;

class Group {
public:
    Group();
    ~Group();

    std::vector<ModelInfo*> models;
    std::vector<Transform*> transforms;
    std::vector<Group*> groups;

};

class ModelInfo {
public:
    ModelInfo(std::string& model_file, std::string& texture_file, const float color[4][3], float shininess);
    ~ModelInfo();

    std::string model_file;
    std::string texture_file;
    float color[4][3]; // diffuse | ambient | specular | emissive 
    float shininess;
};

#endif // GROUP_HPP
