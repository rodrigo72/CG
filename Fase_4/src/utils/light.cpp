#include "light.hpp"
#include <sstream>

PointLight::PointLight(const std::vector<float>& position) {
    this->pos = new std::vector<float>();
    this->pos->push_back(position[0]);
    this->pos->push_back(position[1]);
    this->pos->push_back(position[2]);
    this->pos->push_back(1.0f); // é um um ponto
}

PointLight::~PointLight() {
    delete this->pos;
}

Light::Type PointLight::get_type() const {
    return Type::Point;
}

std::string PointLight::to_string() const {
    std::string result = "PointLight: Position(";
    for (size_t i = 0; i < pos->size(); ++i) {
        result += std::to_string((*pos)[i]);
        if (i != pos->size() - 1) {
            result += ", ";
        }
    }
    result += ")";
    return result;
}

std::vector<float> PointLight::get_pos() {
    std::vector<float> pos;
    for (int i = 0; i < 4; i++) {
        pos.push_back(this->pos->at(i));
    }
    return pos;
}

DirectionalLight::DirectionalLight(const std::vector<float>& direction) {
    this->dir = new std::vector<float>();
    this->dir->push_back(direction[0]);
    this->dir->push_back(direction[1]);
    this->dir->push_back(direction[2]);
    this->dir->push_back(0.0f); // é um vetor
}

DirectionalLight::~DirectionalLight() {
    delete this->dir;
}

Light::Type DirectionalLight::get_type() const {
    return Type::Directional;
}

std::vector<float> DirectionalLight::get_dir() {
    std::vector<float> pos;
    for (int i = 0; i < 4; i++) {
        pos.push_back(this->dir->at(i));
    }
    return pos;
}

std::string DirectionalLight::to_string() const {
    std::string result = "DirectionalLight: Direction(";
    for (size_t i = 0; i < dir->size(); ++i) {
        result += std::to_string((*dir)[i]);
        if (i != dir->size() - 1) {
            result += ", ";
        }
    }
    result += ")";
    return result;
}

SpotLight::SpotLight(const std::vector<float>& position, const std::vector<float>& direction, float cut_off) : cutoff(cut_off) {
    this->pos = new std::vector<float>();
    this->pos->push_back(position[0]);
    this->pos->push_back(position[1]);
    this->pos->push_back(position[2]);
    this->pos->push_back(1.0f); // é um um ponto

    this->dir = new std::vector<float>();
    this->dir->push_back(direction[0]);
    this->dir->push_back(direction[1]);
    this->dir->push_back(direction[2]);
    this->dir->push_back(0.0f); // é um vetor
}

SpotLight::~SpotLight() {
    delete this->dir;
    delete this->pos;
}

Light::Type SpotLight::get_type() const {
    return Type::Spotlight;
}

std::vector<float> SpotLight::get_dir() {
    std::vector<float> pos;
    for (int i = 0; i < 4; i++) {
        pos.push_back(this->dir->at(i));
    }
    return pos;
}

std::vector<float> SpotLight::get_pos() {
    std::vector<float> pos;
    for (int i = 0; i < 4; i++) {
        pos.push_back(this->pos->at(i));
    }
    return pos;
}

float SpotLight::get_cutoff() {
    return this->cutoff;
}

std::string SpotLight::to_string() const {
    std::string result = "SpotLight: Position(";
    for (size_t i = 0; i < pos->size(); ++i) {
        result += std::to_string((*pos)[i]);
        if (i != pos->size() - 1) {
            result += ", ";
        }
    }
    result += "), Direction(";
    for (size_t i = 0; i < dir->size(); ++i) {
        result += std::to_string((*dir)[i]);
        if (i != dir->size() - 1) {
            result += ", ";
        }
    }
    result += "), Cutoff: " + std::to_string(cutoff);
    return result;
}