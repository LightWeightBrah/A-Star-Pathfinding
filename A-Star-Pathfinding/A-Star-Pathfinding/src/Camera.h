#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum MOVEMENT
{
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	STAY_ON_HEIGHT
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

	float movementSpeed		= 6.5f;
	float mouseSensitivity	= 0.1f;

	bool stayOnHeight = false;

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float pitch = 0.0f, float yaw = -90.0f);

	void HandleKeyboardMove(MOVEMENT direction, float deltaTime);
	inline void HandleKeyboardHeight(bool stayOnHeight) { this->stayOnHeight = stayOnHeight; }
	
	void HandleMouse(float xOffset, float yOffset);
	void HandleScrolling(float yOffset);

	void PrintCamera();

	glm::mat4 GetViewMatrix();
	inline const glm::vec3 GetCameraPosition() const { return position; }
	inline float GetFov() const { return fov; }

private:
	void UpdateCamera();

};