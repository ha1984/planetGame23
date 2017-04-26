#pragma once
#include "pch.h"
#include "../../../glm/glm/glm.hpp"
using namespace glm;

class Bullet
{
public:
	vec3 position;
	vec3 startPosition;
	vec3 direction;
	long startTime;
	void move(long time);
	Bullet(vec3 pos, vec3 viewdirection, long t);
	~Bullet();
};

