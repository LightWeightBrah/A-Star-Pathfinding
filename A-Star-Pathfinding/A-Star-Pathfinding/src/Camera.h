#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Movement
{
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
private:
	glm::vec3 position;

	glm::vec3 front		=	glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up		=   glm::vec3(0.0f, 1.0f,  0.0f);
	glm::vec3 right		=   glm::vec3(1.0f, 0.0f,  0.0f);
	glm::vec3 worldUp	=   glm::vec3(0.0f, 1.0f,  0.0f);

	float pitch		=    0.0f;
	float yaw		=  -90.0f;
	float fov		=   45.0f;

	float movementSpeed		= 2.5f;
	float mouseSensitivity	= 0.1f;

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));

	void HandleKeyboard(Movement direction, float deltaTime);
	void HandleMouse(float xOffset, float yOffset);
	void HandleScrolling(float yOffset);

	glm::mat4 GetViewMatrix();
	inline float GetFov() const { return fov; }

private:
	void UpdateCamera();

};