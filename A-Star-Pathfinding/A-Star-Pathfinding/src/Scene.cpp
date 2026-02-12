#include "GL/glew.h"

#include <glm/glm.hpp>

#include "VertexBuffer.h"
#include "ElementBuffer.h"

#include "Scene.h"
#include "Time.h"
#include "InputManager.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Primitives.h"
#include "Shader.h"
#include "Renderer.h"
#include "Entity.h"
#include "Material.h"

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

	auto shader		= std::make_shared<Shader>("res/Shaders/Reflectable.shader");
	auto material	= std::make_shared<Material>(shader);
	auto cubeMesh = Primitives::CreateCube();

	material->SetAmbient(glm::vec3(0.1f))
		.SetDiffuse(glm::vec3(0.8f, 0.6f, 0.0f))
		.SetSpecular(glm::vec3(1.0f))
		.SetShininess(64.0f);

	entity = std::make_unique<Entity>(std::move(cubeMesh), material);
	entity->SetPosition(glm::vec3(0.0f, 5.0f, -5.0f));
	entity->SetScale(glm::vec3(5.0f));
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
	if (!entity)
	{
		std::cout << "ERORR: Scene::Render::Entity is NULL" << std::endl;
		return;
	}

	if (!entity->GetMesh())
	{
		std::cout << "ERORR: Scene::Render::Entity::Mesh is NULL" << std::endl;
		return;
	}


	renderer.Clear(0.05f, 0.05f, 0.05f, 1.0f);
	renderer.DrawEntity(*entity, camera);
}

void Scene::Clear()
{

}
