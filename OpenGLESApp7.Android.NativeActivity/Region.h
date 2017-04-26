#pragma once
#include "Dialogue.h"
#include "../../../glm/glm/glm.hpp"

struct Region {
	dialogue dialog1;
	dialogue dialog2;
	dialogue dialog3;
	dialogue dialog4;
	dialogue dialog5;
	glm::vec3 location1;
	glm::vec3 location2;
	glm::vec3 location3;
};