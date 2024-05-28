#include "config.hpp"
#include "group.hpp"
#include "transform.hpp"
#include "../utils/light.hpp"

#include <cctype>
#include <algorithm>
#include <iostream>

static bool ichar_equals(char a, char b) {
    return std::tolower(static_cast<unsigned char>(a)) ==
        std::tolower(static_cast<unsigned char>(b));
}

static bool iequals(const std::string& a, const std::string& b) {
    return std::equal(a.begin(), a.end(), b.begin(), b.end(), ichar_equals);
}

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

        tinyxml2::XMLElement* xml_lights = root->FirstChildElement("lights");

        if (xml_lights) {
            for (tinyxml2::XMLElement* light = xml_lights->FirstChildElement(); light; light = light->NextSiblingElement()) {
                const char* type = light->Attribute("type");
                if (!type) continue;

                if (strcmp(type, "point") == 0) {

                    float posX = static_cast<float>(atof(light->Attribute("posx")));
                    float posY = static_cast<float>(atof(light->Attribute("posy")));
                    float posZ = static_cast<float>(atof(light->Attribute("posz")));
                    vector<float> position = { posX, posY, posZ };
                    this->lights.push_back(std::make_unique<PointLight>(position));

                }
                else if (strcmp(type, "directional") == 0) {

                    float dirX = static_cast<float>(atof(light->Attribute("dirx")));
                    float dirY = static_cast<float>(atof(light->Attribute("diry")));
                    float dirZ = static_cast<float>(atof(light->Attribute("dirz")));
                    float l = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
                    vector<float> direction = { dirX / l, dirY / l, dirZ / l };
                    this->lights.push_back(std::make_unique<DirectionalLight>(direction));

                }
                else if (strcmp(type, "spot") == 0) {

                    float posX = static_cast<float>(atof(light->Attribute("posx")));
                    float posY = static_cast<float>(atof(light->Attribute("posy")));
                    float posZ = static_cast<float>(atof(light->Attribute("posz")));
                    vector<float> position = { posX, posY, posZ };

                    float dirX = static_cast<float>(atof(light->Attribute("dirx")));
                    float dirY = static_cast<float>(atof(light->Attribute("diry")));
                    float dirZ = static_cast<float>(atof(light->Attribute("dirz")));
                    vector<float> direction = { dirX, dirY, dirZ };

                    float cutoff = static_cast<float>(atof(light->Attribute("cutoff")));
                    this->lights.push_back(std::make_unique<SpotLight>(position, direction, cutoff));
                    
                }
                else {
                    std::cout << "Invalid Light type" << std::endl;
                }
            }

            /*for (const auto& light_ptr : lights) {
                std::cout << light_ptr->to_string() << std::endl;
            }*/
        }

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
                float angle = 0.0f;

                if (strcmp(name, "translate") == 0 && !translate) {
                    translate = true;

                    if (t->Attribute("time") || t->Attribute("align")) {
                        float time = t->Attribute("time") ? static_cast<float>(atof(t->Attribute("time"))) : 0.0f;
                        float align = t->Attribute("align") ? (iequals(t->Attribute("align"), "true") ? true : false) : false;

                        vector<Point> points;
                        for (tinyxml2::XMLElement* p = t->FirstChildElement("point"); p; p = p->NextSiblingElement()) {
                            points.push_back(Point(
                                static_cast<float>(atof(p->Attribute("x"))),
                                static_cast<float>(atof(p->Attribute("y"))),
                                static_cast<float>(atof(p->Attribute("z")))
                            ));
                        }

                        Transform::TransformType type = Transform::TransformType::TRANSLATE_ANIMATION;
                        group->transforms.push_back(new Transform(time, align, points));
                    }
                    else {
                        float x = static_cast<float>(atof(t->Attribute("x")));
                        float y = static_cast<float>(atof(t->Attribute("y")));
                        float z = static_cast<float>(atof(t->Attribute("z")));

                        Transform::TransformType type = Transform::TransformType::TRANSLATE;
                        group->transforms.push_back(new Transform(x, y, z, angle, type));
                    }

                } else if (strcmp(name, "scale") == 0 && !scale) {
                    scale = true;
                    Transform::TransformType type = Transform::TransformType::SCALE;

                    float x = static_cast<float>(atof(t->Attribute("x")));
                    float y = static_cast<float>(atof(t->Attribute("y")));
                    float z = static_cast<float>(atof(t->Attribute("z")));

                    group->transforms.push_back(new Transform(x, y, z, angle, type));

                } else if (strcmp(name, "rotate") == 0 && !rotate) {
                    rotate = true;

                    float x = static_cast<float>(atof(t->Attribute("x")));
                    float y = static_cast<float>(atof(t->Attribute("y")));
                    float z = static_cast<float>(atof(t->Attribute("z")));

                    if (t->Attribute("angle")) {
                        Transform::TransformType type = Transform::TransformType::ROTATE;
                        float angle = static_cast<float>(atof(t->Attribute("angle")));
                        group->transforms.push_back(new Transform(x, y, z, angle, type));
                    } else {
                        Transform::TransformType type = Transform::TransformType::ROTATE_ANIMATION;
                        float time = static_cast<float>(atof(t->Attribute("time")));
                        group->transforms.push_back(new Transform(x, y, z, time));
                    }
                }
            }
        }

        // Models
        tinyxml2::XMLElement* xml_models = xml_group->FirstChildElement("models");
        if (xml_models) {
            for (tinyxml2::XMLElement* m = xml_models->FirstChildElement("model"); m; m = m->NextSiblingElement()) {

                std::string model_file = m->Attribute("file");
                if (model_file.empty()) continue;

                std::string texture_file;
                tinyxml2::XMLElement* model_texture = m->FirstChildElement("texture");

                if (model_texture) {
                    texture_file = model_texture->Attribute("file");
                }
                
                float shininess = 0;
                float color_settings[4][3] = {
                    {200, 200, 200},
                    {50, 50, 50},
                    {0, 0, 0},
                    {0, 0, 0}
                };
                tinyxml2::XMLElement* model_color = m->FirstChildElement("color");

                if (model_color) {

                    tinyxml2::XMLElement* diffuse = model_color->FirstChildElement("diffuse");
                    if (diffuse) {
                        color_settings[0][0] = static_cast<float>(atof(diffuse->Attribute("R")));
                        color_settings[0][1] = static_cast<float>(atof(diffuse->Attribute("G")));
                        color_settings[0][2] = static_cast<float>(atof(diffuse->Attribute("B")));
                    }

                    tinyxml2::XMLElement* ambient = model_color->FirstChildElement("ambient");
                    if (ambient) {
                        color_settings[1][0] = static_cast<float>(atof(ambient->Attribute("R")));
                        color_settings[1][1] = static_cast<float>(atof(ambient->Attribute("G")));
                        color_settings[1][2] = static_cast<float>(atof(ambient->Attribute("B")));
                    }

                    tinyxml2::XMLElement* specular = model_color->FirstChildElement("specular");
                    if (specular) {
                        color_settings[2][0] = static_cast<float>(atof(specular->Attribute("R")));
                        color_settings[2][1] = static_cast<float>(atof(specular->Attribute("G")));
                        color_settings[2][2] = static_cast<float>(atof(specular->Attribute("B")));
                    }
                    

                    tinyxml2::XMLElement* emissive = model_color->FirstChildElement("emissive");
                    if (emissive) {
                        color_settings[3][0] = static_cast<float>(atof(emissive->Attribute("R")));
                        color_settings[3][1] = static_cast<float>(atof(emissive->Attribute("G")));
                        color_settings[3][2] = static_cast<float>(atof(emissive->Attribute("B")));
                    }

                    tinyxml2::XMLElement* xml_shininess = model_color->FirstChildElement("shininess");
                    if (xml_shininess)
                        shininess = static_cast<float>(atof(xml_shininess->Attribute("value")));
                }

                ModelInfo* model_info = new ModelInfo(model_file, texture_file, color_settings, shininess);
                group->models.push_back(model_info);
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