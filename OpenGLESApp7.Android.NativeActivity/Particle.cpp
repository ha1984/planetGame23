#include "pch.h"
#include "Particle.h"


Particle::Particle(glm::vec3 pos, glm::vec3 dir, long time) : lifeTime(0)
{
	startTime = time;
	startPosition = pos;
	position = pos;
	direction = dir;
	velocity = 0.00000001f;
}

void Particle::move(long time)
{
	float t = float(time - startTime);

	position.x = startPosition.x + t*velocity*direction.x;
	position.y = startPosition.y + t*velocity*direction.y; // -0.000002*t*t;
	position.z = startPosition.z + t*velocity*direction.z;

	velocity = velocity*0.9f;

	lifeTime += 1;
}


Particle::~Particle()
{
}
