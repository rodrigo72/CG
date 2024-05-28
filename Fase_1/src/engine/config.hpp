#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <string>

class Config {
public:
    Config::Config(const char* file_path);
    ~Config();

    std::vector<std::string> models;

    void set_cam_position(float x, float y, float z);
    float get_x_pos_cam() const;
    float get_y_pos_cam() const;
    float get_z_pos_cam() const;
    float get_x_look_at() const;
    float get_y_look_at() const;
    float get_z_look_at() const;
    float get_x_up() const;
    float get_y_up() const;
    float get_z_up() const;
    float get_fov() const;
    float get_near() const;
    float get_far() const;

private:
    float pos_cam[3];
    float look_at[3];
    float up[3];
    float projection[3];
};

#endif // CONFIG_HPP
