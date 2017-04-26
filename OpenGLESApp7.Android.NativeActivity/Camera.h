#pragma once
#include <vector>
#include "../../../glm/glm/glm.hpp"
#include "../../../glm/glm/gtc/matrix_transform.hpp"
class Camera
{
	float oldMousePosition;
	float oldMousePositionY;
public:
	glm::vec3 UP;
	//std::vector<Bullet*> bulletVec;
	glm::vec3 viewDirection;
	glm::vec3 position;
	float angletank;
	float anglegun;
	glm::vec3 playerPosition;
	Camera();
	void mouseUpdate(const float& newMousePosition);
	void keyUpdate(const float& newMousePosition, int& height);
	const glm::mat4 getWorldToViewMatrix();
};

