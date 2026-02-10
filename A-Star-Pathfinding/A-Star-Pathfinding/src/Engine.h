#pragma once
#include "Event.h"

class GLFWwindow;
class Scene;
class Renderer;

class Engine
{
private:
	GLFWwindow*						window;
	std::unique_ptr<Renderer>		renderer;
	std::unique_ptr<Scene>			scene;

	Event onCloseEvent;
	Event onMenuEvent;

	bool isMenuOpen = false;

	const unsigned int WINDOW_WIDTH = 1200;
	const unsigned int WINDOW_HEIGHT = 800;

	bool Init();
	void ProcessInput();
	void Update(float deltaTime);
	void Render();
	void Shutdown();

	void SetCallbacks();
	void SubsribeToEvents();
	void OpenMenu();

public:
	Engine();
	~Engine();

	void Run();

};