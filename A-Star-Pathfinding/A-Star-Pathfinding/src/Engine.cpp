#include <iostream>

#include <assimp/version.h>

#include "Engine.h"
#include "Scene.h"
#include "Renderer.h"
#include "Time.h"
#include "InputManager.h"

Engine::Engine()
	: window(nullptr), isMenuOpen(false)
{

}
Engine::~Engine()
{

}

void Engine::Run()
{
	if (!Init())
		return;

	while (!glfwWindowShouldClose(window))
	{
		Time::Update();

		ProcessInput();

		Update(Time::deltaTime);

		Render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		InputManager::Clear();
	}

	Shutdown();
}

bool Engine::Init()
{
	if (!glfwInit())
	{
		std::cout << "ERROR: Failed to initialize GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	float xScale, yScale;
	glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xScale, &yScale);
	float displayScale = xScale;

	window = glfwCreateWindow(
		static_cast<int>((WINDOW_WIDTH  * displayScale)),
		static_cast<int>((WINDOW_HEIGHT * displayScale)),
		"SunFinder", NULL, NULL);

	if (!window)
	{
		std::cout << "ERROR: Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR: Failed to initalize GLEW" << std::endl;
		return false;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;
	GLCall(glEnable(GL_DEPTH_TEST));
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	SetCallbacks();
	SubsribeToEvents();
	
	scene.Init();

	return true;
}

void Engine::Update(float deltaTime)
{
	scene.Update();
}

void Engine::ProcessInput()
{
	if (InputManager::IsKeyDown(GLFW_KEY_ESCAPE))
		onCloseEvent.Invoke();

	if (InputManager::IsKeyDown(GLFW_KEY_TAB))
		onMenuEvent.Invoke();

	scene.ProcessInput();
}

void Engine::Render()
{

}

void Engine::Shutdown()
{
	scene.Clear();

	if (window)
		glfwDestroyWindow(window);

	glfwTerminate();
}

void Engine::SetCallbacks()
{
	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });

	glfwSetCursorPosCallback(window, InputManager::OnMouse);
	glfwSetKeyCallback(window, InputManager::OnSingleKey);
	glfwSetScrollCallback(window, InputManager::OnScroll);
}

void Engine::SubsribeToEvents()
{
	onCloseEvent.AddListener([this]() { glfwSetWindowShouldClose(window, true);});
	onMenuEvent.AddListener([this]()  { OpenMenu(); });
}

void Engine::OpenMenu()
{
	isMenuOpen = !isMenuOpen;

	if (isMenuOpen)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		InputManager::SetFirstMouse(true);
	}
}
