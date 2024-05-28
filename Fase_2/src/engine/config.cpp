#include "config.hpp"
#include "group.hpp"
#include "transform.hpp"


Config::~Config() {
    delete group;
}

Config::Config(const char* file_path) {

    // initialize default values
    for (int i = 0; i < 3; ++i) {
        pos_cam[i] = 0.0f;
        look_at[i] = 0.0f;
        up[i] = 0.0f;
        projection[i] = 0.0f;
    }

    // load xml document
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(file_path) == tinyxml2::XML_SUCCESS) {
        tinyxml2::XMLElement* root = doc.FirstChildElement("world");

        tinyxml2::XMLElement* camera = root->FirstChildElement("camera");

        tinyxml2::XMLElement* pos_camera = camera->FirstChildElement("position");
        tinyxml2::XMLElement* look_at_camera = camera->FirstChildElement("lookAt");
        tinyxml2::XMLElement* up_camera = camera->FirstChildElement("up");
        tinyxml2::XMLElement* projection_camera = camera->FirstChildElement("projection");

        pos_cam[0] = pos_camera->FloatAttribute("x");
        pos_cam[1] = pos_camera->FloatAttribute("y");
        pos_cam[2] = pos_camera->FloatAttribute("z");

        look_at[0] = look_at_camera->FloatAttribute("x");
        look_at[1] = look_at_camera->FloatAttribute("y");
        look_at[2] = look_at_camera->FloatAttribute("z");

        up[0] = up_camera->FloatAttribute("x");
        up[1] = up_camera->FloatAttribute("y");
        up[2] = up_camera->FloatAttribute("z");

        projection[0] = projection_camera->FloatAttribute("fov");
        projection[1] = projection_camera->FloatAttribute("near");
        projection[2] = projection_camera->FloatAttribute("far");

        group = parse_groups(root);
    }
    else {
        group = new Group();
    }
}

Group* Config::parse_groups(tinyxml2::XMLElement* xml_group) {
    if (xml_group) {
        Group* group = new Group();

        // Transforms
        tinyxml2::XMLElement* xml_transform = xml_group->FirstChildElement("transform");
        if (xml_transform) {

            bool translate = false;
            bool scale = false;
            bool rotate = false;

            for (tinyxml2::XMLElement* t = xml_transform->FirstChildElement(); t; t = t->NextSiblingElement()) {

                const char* name = t->Value();
                Transform::TransformType type;
                float angle = 0.0f;

                if (strcmp(name, "translate") == 0 && !translate) {
                    type = Transform::TransformType::TRANSLATE;
                    translate = true;
                } else if (strcmp(name, "scale") == 0 && !scale) {
                    type = Transform::TransformType::SCALE; 
                    scale = true;
                } else if (strcmp(name, "rotate") == 0 && !rotate) {
                    type = Transform::TransformType::ROTATE; 
                    angle = static_cast<float>(atof(t->Attribute("angle")));
                    rotate = true;
                } else { 
                    continue; 
                }

                float x = static_cast<float>(atof(t->Attribute("x")));
                float y = static_cast<float>(atof(t->Attribute("y")));
                float z = static_cast<float>(atof(t->Attribute("z")));

                group->transforms.push_back(new Transform(x, y, z, angle, type));
            }
        }

        // Models
        tinyxml2::XMLElement* xml_models = xml_group->FirstChildElement("models");
        if (xml_models) {
            for (tinyxml2::XMLElement* m = xml_models->FirstChildElement("model"); m; m = m->NextSiblingElement()) {
                group->models.push_back((m->Attribute("file")));
            }
        }

        // Groups
        for (tinyxml2::XMLElement* g = xml_group->FirstChildElement("group"); g; g = g->NextSiblingElement("group")) {
            Group* child = parse_groups(g);
            group->groups.push_back(child);
        }

        return group;
    }
    return NULL;
}


void Config::set_cam_position(float x, float y, float z) {
    pos_cam[0] = x;
    pos_cam[1] = y;
    pos_cam[2] = z;
}


float Config::get_x_pos_cam() const { return pos_cam[0]; }
float Config::get_y_pos_cam() const { return pos_cam[1]; }
float Config::get_z_pos_cam() const { return pos_cam[2]; }

float Config::get_x_look_at() const { return look_at[0]; }
float Config::get_y_look_at() const { return look_at[1]; }
float Config::get_z_look_at() const { return look_at[2]; }

float Config::get_x_up() const { return up[0]; }
float Config::get_y_up() const { return up[1]; }
float Config::get_z_up() const { return up[2]; }

float Config::get_fov() const { return projection[0]; }
float Config::get_near() const { return projection[1]; }
float Config::get_far() const { return projection[2]; }