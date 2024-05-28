#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <vector>
#include <string>

class Light {
public:
    enum class Type {
        Point,
        Directional,
        Spotlight
    };

    virtual ~Light() {}
    virtual Type get_type() const = 0;
    virtual std::string to_string() const = 0;
};

class PointLight : public Light {
public:
    PointLight(const std::vector<float>& position);
    ~PointLight();
    Type get_type() const override;
    std::string to_string() const override;
    std::vector<float> get_pos();

private:
    std::vector<float>* pos;
};

class DirectionalLight : public Light {
public:
    DirectionalLight(const std::vector<float>& direction);
    ~DirectionalLight();
    Type get_type() const override;
    std::string to_string() const override;
    std::vector<float> get_dir();

private:
    std::vector<float>* dir;
};

class SpotLight : public Light {
public:
    SpotLight(const std::vector<float>& position, const std::vector<float>& direction, float cut_off);
    ~SpotLight();
    Type get_type() const override;
    std::string to_string() const override;
    std::vector<float> get_pos();
    std::vector<float> get_dir();
    float get_cutoff();

private:
    std::vector<float>* pos;
    std::vector<float>* dir;
    float cutoff;
};

#endif // LIGHT_HPP