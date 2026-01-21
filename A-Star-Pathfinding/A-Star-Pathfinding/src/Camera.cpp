#include "Camera.h"

Camera::Camera(glm::vec3 position) : position(position)
{
	UpdateCamera();
}

void Camera::HandleKeyboard(MOVEMENT direction, float deltaTime)
{
	float speed = this->movementSpeed * deltaTime;

	if (direction == MOVEMENT::FORWARD)
		position += front		*	speed;
	if (direction == MOVEMENT::BACKWARD)
		position -= front		*	speed;
	if (direction == MOVEMENT::LEFT)
		position -= right		*	speed;
	if (direction == MOVEMENT::RIGHT)
		position += right		*	speed;
	if (direction == MOVEMENT::UP)
		position += worldUp		*	speed;
	if (direction == MOVEMENT::DOWN)
		position -= worldUp		*	speed;

}

void Camera::HandleMouse(float xOffset, float yOffset)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	UpdateCamera();
}

void Camera::HandleScrolling(float yOffset)
{
	fov -= (float)yOffset;

	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::UpdateCamera()
{
	glm::vec3 direction;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front		= glm::normalize(direction);


	// normalize the vectors, cause their length gets closer to 
	// 0 the more you look up or down which results in slower movement
	right		= glm::normalize(glm::cross(front, worldUp));
	up			= glm::normalize(glm::cross(right, front));
}