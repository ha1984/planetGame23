#pragma once
#include "../../../glm/glm/glm.hpp"
struct Bomb {
	glm::vec3 position;
	bool detonate = false;
};