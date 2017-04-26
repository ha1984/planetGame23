#include "Bullet.h"


void Bullet::move(long time)
{
	float t = (float)(time - startTime) / 20000000.0f;
	position.x = startPosition.x + t*direction.x;
	position.y = startPosition.y + t*direction.y;
	position.z = startPosition.z + t*direction.z;
}

Bullet::Bullet(vec3 pos, vec3 viewdirection, long t)
{
	startTime = t;
	startPosition = pos + glm::vec3(0.0, 3.0, 0.0);
	position = startPosition;
	direction = viewdirection;
}


Bullet::~Bullet()
{
}
