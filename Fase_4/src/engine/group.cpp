#include "group.hpp"

Group::Group() {}

Group::~Group() {
	models.clear();

	for (Transform* t : transforms)
		delete t;
	transforms.clear();
	
	for (ModelInfo* m : models)
		delete m;

	for (Group* group : groups)
		delete group;

	groups.clear();
}

ModelInfo::ModelInfo(
	std::string& model_file, 
	std::string& texture_file, 
	const float color[4][3],
	float shininess
) : model_file(model_file), texture_file(texture_file), shininess(shininess) 
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 3; ++j) {
			this->color[i][j] = color[i][j];
		}
	}
}

ModelInfo::~ModelInfo() {}