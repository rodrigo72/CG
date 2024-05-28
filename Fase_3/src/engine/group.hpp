#ifndef GROUP_HPP
#define GROUP_HPP

#include <vector>
#include <string>
#include "transform.hpp"
#include "group.hpp"

class Group {
public:
    Group();
    ~Group();

    std::vector<std::string> models;
    std::vector<Transform*> transforms;
    std::vector<Group*> groups;

};

#endif // GROUP_HPP
