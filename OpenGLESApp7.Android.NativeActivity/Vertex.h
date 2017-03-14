#pragma once
#include "../OpenGLESApp7.Android.NativeActivity/include/MathHelper.h"
//#include <glm\glm.hpp>


struct Vertex {
	MathHelper::vec3 position = MathHelper::vec3(0.0f,0.0f,0.0f);
	MathHelper::vec3 normal = MathHelper::vec3(0.0f, 0.0f, 0.0f);
	MathHelper::vec2 texture = MathHelper::vec2(0.0f, 0.0f);
};