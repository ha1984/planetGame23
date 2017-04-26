#pragma once
#include "Bomb.h"
#include "include/ShapeData.h"
#include <vector>
#include "Bullet.h"
class Collision
{
public:
	static Bomb test(ShapeData cube, glm::mat4 modelToWorldMatrix, std::vector<Bullet*>& bulletVec, glm::vec3 playerPosition);
	Collision();
	~Collision();
};

