#include "pch.h"
#include "Camera.h"

using namespace glm;
Camera::Camera() : viewDirection(0.0f, 0.0f, 1.0f),
UP(0.0f, 1.0f, 0.0f),
position(0.0, 5.0, -10.0),
anglegun(0), angletank(0),
playerPosition(0.0,0.0,0.0)
{
	
}

void Camera::keyUpdate(const float& newMousePosition, int& height) {
	if (newMousePosition > (float)height/2) {
		playerPosition.x -= 0.1*viewDirection.x;
		playerPosition.z -= 0.1*viewDirection.z;
	}
	else {
		playerPosition.x += 0.1*viewDirection.x;
		playerPosition.z += 0.1*viewDirection.z;
	}

	//float MouseDeltaY = newMousePosition - oldMousePositionY;

	//MouseDeltaY = MouseDeltaY * 0.03f;
	//MouseDelta.y = (double)MouseDelta.y * 0.03;

	//position.x -= 0.3*viewDirection.x;
	//position.z -= 0.3*viewDirection.z;
	//oldMousePositionY = newMousePosition;
}

void Camera::mouseUpdate(const float& newMousePosition)
{

	if (std::abs(newMousePosition - oldMousePosition) > 30.0f) {
		oldMousePosition = newMousePosition;
	}

	float MouseDeltaX = newMousePosition - oldMousePosition;

	MouseDeltaX = MouseDeltaX * 0.003f;
	//MouseDelta.y = (double)MouseDelta.y * 0.03;

	anglegun -= MouseDeltaX;
	//rotate in x-direction:
	viewDirection = mat3(rotate(mat4(), anglegun, vec3(0.0, 1.0, 0.0)))*vec3(0.0, 0.0, 1.0);
	//viewDirection = normalize(glm::mat3(glm::rotate(glm::mat4(), -MouseDeltaX, UP)) * viewDirection);
	//gunDirection = glm::mat3(glm::rotate(glm::mat4(), -MouseDeltaX, UP)) * gunDirection;
	

	//glm::vec3 toRotateAround = glm::cross(viewDirection, UP);
	////rotate in y-direction:
	//viewDirection = glm::mat3(glm::rotate(glm::mat4(),-MouseDelta.y, toRotateAround)) * viewDirection;
	//UP = glm::mat3(glm::rotate(-MouseDelta.y, toRotateAround)) * UP;

	oldMousePosition = newMousePosition;
}

const glm::mat4 Camera::getWorldToViewMatrix()
{
	return glm::lookAt(position, position + viewDirection, UP); //2nd argument of lookAt: camera looks at this point! (position + viewDirection = lookat point in world-coordinates)
}



