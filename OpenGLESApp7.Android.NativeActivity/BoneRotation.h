#pragma once
#include "../../../glm/glm/glm.hpp"
#include "../../../glm/glm/gtc/matrix_transform.hpp"
#include <vector>
using namespace glm;

struct BoneRotation
{
	std::vector<mat4> boneRot;
};

