#pragma once
#include "../../../glm/glm/glm.hpp"
class Particle
{
public:
	glm::vec3 startPosition;
	long startTime;
	int lifeTime;
	glm::vec3 position;
	glm::vec3 direction;
	float velocity;
	Particle(glm::vec3 pos, glm::vec3 dir, long time);
	void move(long time);
	~Particle();
};

