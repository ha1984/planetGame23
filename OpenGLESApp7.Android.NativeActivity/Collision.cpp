#include "pch.h"
#include "Collision.h"
using namespace glm;

Collision::Collision()
{
}

Bomb Collision::test(ShapeData cube, glm::mat4 modelToWorldMatrix, std::vector<Bullet*>& bulletVec, vec3 playerPosition) {
	Bomb bomb;

	std::vector<vec3> worldPositions(6);
	std::vector<vec3> worldN(6);
	std::vector<vec3> world(6);

	for (int j = 0; j < 6; j++) {
		worldPositions[j] = vec3(modelToWorldMatrix*vec4(vec3(cube.vertices[4 * j].position.m[0], cube.vertices[4 * j].position.m[1], cube.vertices[4 * j].position.m[2]), 1.0f));
		//worldN[j] = vec3(modelToWorldMatrix*vec4(cube.vertices[4 * j].normal, 0.0f)); // works without nonuniform scaling
		worldN[j].x = cube.vertices[4 * j].normal.m[0];
		worldN[j].y = cube.vertices[4 * j].normal.m[1];
		worldN[j].z = cube.vertices[4 * j].normal.m[2];// works without rotations
	}

	std::vector<Bullet*>::iterator it;
	for (it = bulletVec.begin(); it != bulletVec.end();) {
		for (int j = 0; j < 6; j++) {
			world[j] = (*it)->position - worldPositions[j];
		}
		if ((dot(world[0], worldN[0]) < 0) && (dot(world[1], worldN[1]) < 0) && (dot(world[2], worldN[2]) < 0)
			&& (dot(world[3], worldN[3]) < 0) && (dot(world[4], worldN[4]) < 0) && (dot(world[5], worldN[5]) < 0))
		{
			bomb.detonate = true;
			bomb.position = (*it)->position;
			delete (*it);
			it = bulletVec.erase(it);
		}
		else if (length((*it)->position - playerPosition) > 60) {
			delete (*it);
			it = bulletVec.erase(it);
		}
		else {
			++it;
		}
	}

	return bomb;
};


Collision::~Collision()
{
}
