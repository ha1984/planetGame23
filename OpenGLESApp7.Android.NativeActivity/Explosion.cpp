#include "pch.h"
#include "Explosion.h"



Explosion::Explosion(glm::vec3 pos, long time)
{
	startTime = time;
	position = pos;
}

//spawn particles:
void Explosion::spawn(long time)
{
	glm::vec3 direction;

	//if (elapsedTime < 10) {
		for (int i = 0; i < 100; i++) {
			direction = glm::sphericalRand(2.0f);
			Particle* particle = new Particle(position, direction, time);
			particleVec.push_back(particle);
		}
	//}

	elapsedTime += 1;
}


Explosion::~Explosion()
{
}
