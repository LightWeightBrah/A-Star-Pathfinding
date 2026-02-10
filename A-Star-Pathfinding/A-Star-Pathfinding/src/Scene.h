#pragma once

class Camera;
class Renderer;

class Scene
{
private:
	//Shader		basicShader;
	Camera      camera;

	//Entity		solaireEntity;
	//Astar       aStar;

public:
	Scene();
	~Scene();

	void Init(float windowWidth, float windowHeight);
	void OnWindowResize(float windowWidth, float windowHeight);
	void ProcessInput();
	void Update();
	void Render(Renderer& renderer);
	void Clear();
	
};