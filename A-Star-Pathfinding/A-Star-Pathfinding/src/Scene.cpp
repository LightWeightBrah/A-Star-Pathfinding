#include <glm/glm.hpp>

#include "Scene.h"
#include "Time.h"
#include "InputManager.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Primitives.h"
#include "Shader.h"
#include "Renderer.h"
#include "Entity.h"

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

	ResourceManager::LoadTexture("cube_container", "res/Textures/container.jpg");
	ResourceManager::LoadTexture("cube_chad"     , "res/Textures/chad.png"     );

	shader	= std::make_unique<Shader>("res/Shaders/Basic.shader");
	entity->	= Primitives::CreateCube();
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
	if (!cubeMesh || !cubeShader)
		return;


	cubeShader->Bind();
	cubeMesh  ->BindTextures(*cubeShader);

	cubeShader->SetUniformMatrix4fv("projection", camera.GetProjectionMatrix());
	cubeShader->SetUniformMatrix4fv("view"		, camera.GetViewMatrix());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 5.0f, 5.0f));
	cubeShader->SetUniformMatrix4fv("model", model);
	
	cubeShader->SetUniform3f("objectColor", glm::vec3(1.0f, 0.5f, 1.0f));

	renderer.Draw(cubeMesh->GetVAO(), cubeMesh->GetEBO(), *cubeShader);
}

void Scene::Clear()
{

}
