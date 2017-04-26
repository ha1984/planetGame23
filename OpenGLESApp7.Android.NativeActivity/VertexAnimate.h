#pragma once
//#include "../OpenGLESApp7.Android.NativeActivity/include/MathHelper.h"
//#include "../OpenGLESApp7.Shared/SimpleRenderer.h"
#include "../../../glm/glm/glm.hpp"
#include "../../../glm/glm/gtc/matrix_transform.hpp"
#include "../../../glm/glm/gtc/type_ptr.hpp"


class VertexAnimate {
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;
	glm::vec3 weights;
	glm::vec3 index;
	VertexAnimate();
};