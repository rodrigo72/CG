#include "group.hpp"

Group::Group() {}

Group::~Group() {
	models.clear();

	for (Transform* t : transforms)
		delete t;
	transforms.clear();

	for (Group* group : groups)
		delete group;

	groups.clear();
}