#pragma once
#include "../../../glm/glm/glm.hpp"
#include "../../../glm/glm/gtc/random.hpp"
#include "Particle.h"
#include <vector>

class Explosion
{
public:
	std::vector<Particle*> particleVec;
	glm::vec3 position;
	long startTime;
	int elapsedTime;
	Explosion(glm::vec3 pos, long time);
	void spawn(long time);
	~Explosion();
};

