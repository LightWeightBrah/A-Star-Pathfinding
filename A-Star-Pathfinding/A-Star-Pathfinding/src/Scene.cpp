#include <glm/glm.hpp>

#include "Scene.h"
#include "Time.h"
#include "InputManager.h"
#include "Camera.h"
#include "ResourceManager.h"

Scene::Scene() 
	: camera(glm::vec3(0.0f, 0.0f, 0.0f))
{

}

Scene::~Scene()
{

}

void Scene::Init(float windowWidth, float windowHeight)
{
	OnWindowResize(windowWidth, windowHeight);

	ResourceManager::LoadTexture("res/Textures/container.jpg", "cube_container");
	ResourceManager::LoadTexture("res/Textures/chad.png"     , "cube_chad");

}

void Scene::OnWindowResize(float windowWidth, float windowHeight)
{
	camera.SetViewportSize(windowWidth, windowHeight);
}

void Scene::ProcessInput()
{
	float deltaTime				= Time::deltaTime;
	glm::vec2 mouseDelta		= InputManager::GetMouseDelta();

	camera.HandleMouseMovement(mouseDelta.x, mouseDelta.y);
	float scroll = InputManager::GetScrollOffset();
	if (scroll != 0.0f)
		camera.HandleScrolling(scroll);

	camera.HandleStayOnHeight(InputManager::IsKeyPressed(GLFW_KEY_LEFT_ALT));

	if (InputManager::IsKeyPressed(GLFW_KEY_W))
		camera.HandleKeyboardMove(MOVEMENT::FORWARD	, deltaTime);
	if (InputManager::IsKeyPressed(GLFW_KEY_S))
		camera.HandleKeyboardMove(MOVEMENT::BACKWARD, deltaTime);
	if (InputManager::IsKeyPressed(GLFW_KEY_A))
		camera.HandleKeyboardMove(MOVEMENT::LEFT	, deltaTime);
	if (InputManager::IsKeyPressed(GLFW_KEY_D))
		camera.HandleKeyboardMove(MOVEMENT::RIGHT	, deltaTime);

	if (InputManager::IsKeyPressed(GLFW_KEY_SPACE))
		camera.HandleKeyboardMove(MOVEMENT::UP		, deltaTime);
	if (InputManager::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		camera.HandleKeyboardMove(MOVEMENT::DOWN	, deltaTime);

}

void Scene::Update()
{

}

void Scene::Render(Renderer& renderer)
{

}

void Scene::Clear()
{

}
