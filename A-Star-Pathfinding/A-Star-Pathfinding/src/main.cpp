#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "main.h"
#include "Node.h"
#include "Grid.h"
#include "AStar.h"
#include "App.h"
#include "Renderer.h"
#include "Camera.h"
#include "Model.h"

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/version.h>

const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 800;

const unsigned int WALL_HEIGHT = 2;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = WINDOW_WIDTH / 2;
float lastY = WINDOW_HEIGHT / 2;

float astarInterval	= 0.1f;
float astarCounter	= 0.0f;

float bonesChangeInterval = 0.5f;
float bonesChangeCounter  = 0.0f;
int   boneIndex = 0;

int modelX = 0, modelZ = 19;

bool firstMouse = true;

Camera camera(glm::vec3(9.4f, 34.2f, 30.8f), -60.1f, -89.9);

AStar aStar("grid.txt");


void OnWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void HandleInput(GLFWwindow* window)
{
	const float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.HandleKeyboardHeight(true);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		camera.HandleKeyboardHeight(false);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.HandleKeyboardMove(MOVEMENT::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.HandleKeyboardMove(MOVEMENT::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.HandleKeyboardMove(MOVEMENT::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.HandleKeyboardMove(MOVEMENT::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.HandleKeyboardMove(MOVEMENT::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.HandleKeyboardMove(MOVEMENT::DOWN, deltaTime);
}

void OnSingleKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_R:
			aStar.Reset(modelX, modelZ);
			break;

		case GLFW_KEY_F:
			aStar.Reset(modelX, modelZ);
			aStar.FindPathFull();
			break;

		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;
		}
	}
}

void OnMouse(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xoffset = xPos  - lastX;
	float yoffset = lastY - yPos;
	
	lastX = xPos;
	lastY = yPos;

	camera.HandleMouse(xoffset, yoffset);
}

void OnScroll(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.HandleScrolling(yOffset);
}

float verticies[] = {
	//positions				//colors			//textures
	//front
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	//top right		
	 0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	//bottom right	
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	//bottom left	
	-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 0.0f,   0.0f, 1.0f,	//top left		

	//back
	 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	//top right		
	 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	//bottom right	
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	//bottom left	
	-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f,   0.0f, 1.0f,	//top left		

	//up
	 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	//top right		
	 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	//bottom right	
	-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	//bottom left	
	-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f,   0.0f, 1.0f,	//top left		

	//bottom
	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	//top right		
	 0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	//bottom right	
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	//bottom left	
	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,   0.0f, 1.0f,	//top left

	//right
	 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	//top right		
	 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	//bottom right	
	 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	//bottom left	
	 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 0.0f,   0.0f, 1.0f,	//top left		

	//left
	-0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	//top right		
	-0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	//bottom right	
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	//bottom left	
	-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f,   0.0f, 1.0f,	//top left		
};

unsigned int indicies[] = {
	//front
	0,  1,  2, //first triangle  
	2,  3,  0, //2nd triangle	   
	//back					   
	4,  5,  6, 				   
	6,  7,  4,				   
	//up
	8,  9,  10,
	10, 11,  8,
	//down
	12, 13, 14,
	14, 15, 12,
	////right
	16, 17, 18,
	18, 19, 16,
	//left
	20, 21, 22,
	22, 23, 20,
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SunFinder", NULL, NULL);
	if (!window)
	{
		std::cout << "ERROR: Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR: Failed to initalize GLEW" << std::endl;
		return -1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;
	int nrAttributes;
	GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes));
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	Assimp::Importer importer;
	std::cout << "Wersja Assimpa: " << aiGetVersionMajor() << std::endl;

	GLCall(glEnable(GL_DEPTH_TEST));
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, OnWindowResized);
	glfwSetCursorPosCallback(window, OnMouse);
	glfwSetKeyCallback(window, OnSingleKey);
	glfwSetScrollCallback(window, OnScroll);

	Renderer renderer;


	glEnable(GL_DEPTH_TEST);
	renderer.Clear(0.05f, 0.05f, 0.05f, 1.0f);
	glfwSwapBuffers(window);
	glfwPollEvents();

	{
		Shader shader("res/shaders/Basic.shader");

		VertexArray   VAO;
		VertexBuffer  VBO(verticies, sizeof(verticies));
		ElementBuffer EBO(indicies, sizeof(indicies));

		VAO.AddAttrib(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
		VAO.AddAttrib(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		VAO.AddAttrib(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		Texture texture1("res/Textures/container.jpg");
		Texture texture2("res/Textures/chad.png");

		shader.SetUniform1i("texture1", 0);
		shader.SetUniform1i("texture2", 1);

		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();
		shader.Unbind();

		//Model characterModel("res/Models/solair masterpiece/Solair Final Model.obj", false);
		Model characterModel("res/Models/vampire/dancing_vampire.dae", false);
		Shader characterShader("res/shaders/Model.shader");
		characterShader.Unbind();

		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			HandleInput(window);

			renderer.Clear(0.05f, 0.05f, 0.05f, 1.0f);

			shader.Bind();
			texture1.Bind();
			texture2.Bind(1);

			glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
			shader.SetUniformMatrix4fv("projection", projection);

			glm::mat4 view = camera.GetViewMatrix();
			shader.SetUniformMatrix4fv("view", view);

			aStar.FindPathBySteps(deltaTime);


			for (int z = 0; z < aStar.height; z++)
			{
				for (int x = 0; x < aStar.width; x++)
				{
					bool isWall = false;

					glm::vec3 color;
					Node* currentNode = &aStar.grid[z][x];

					shader.Bind();
					texture1.Bind();
					texture2.Bind(1);

					if (currentNode == aStar.GetStartNode())
					{
						color = glm::vec3(1.0f, 1.0f, 1.0f);
					}
					else if (currentNode == aStar.GetEndNode())
					{
						color = glm::vec3(1.0f, 1.0f, 1.0f);
					}
					else
					{
						switch (currentNode->cellType)
						{
						case CELL::WALL:	color = glm::vec3(1.0f, 0.0f, 0.0f); isWall = true; break;
						case CELL::ROUTE:	color = glm::vec3(1.0f, 1.0f, 0.0f);				break;
						case CELL::OPEN:	color = glm::vec3(0.36f, 0.84f, 0.42f);				break;
						case CELL::CLOSED:	color = glm::vec3(0.0f, 0.0f, 0.3f);				break;
						default:			color = glm::vec3(0.6f, 0.0f, 0.6f);				break;
						}
					}

					shader.SetUniform3f("objectColor", color.x, color.y, color.z);

					int maxHeight = isWall ? WALL_HEIGHT : 1;
					for (int y = 0; y < maxHeight; y++)
					{
						glm::mat4 model = glm::mat4(1.0f);
						model = glm::translate(model, glm::vec3(x, y, z));

						shader.SetUniformMatrix4fv("model", model);
						renderer.Draw(VAO, EBO, shader);
					}

					
				}
			}

			characterShader.Bind();
			characterShader.SetUniform1i("texture_diffuse1", 0);
			characterShader.SetUniformMatrix4fv("projection", projection);
			characterShader.SetUniformMatrix4fv("view", view);

			/*glm::mat4 modelMatrix = glm::mat4(1.0f);
			aStar.TravelWithModel(modelX, modelZ, deltaTime);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(modelX, 0.37, modelZ));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.035f, 0.035f, 0.035f));
			characterShader.SetUniformMatrix4fv("model", modelMatrix);
			renderer.DrawModel(characterModel, characterShader);*/

			
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.37, 20));
			//modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.005f, 0.005f, 0.005f));
			
			/*bonesChangeCounter += deltaTime;

			if (bonesChangeCounter >= bonesChangeInterval)
			{
				int displayIndex = boneIndex % characterModel.GetTotalBones();
				characterShader.SetUniform1i("displayBoneIndex", displayIndex);
				boneIndex++;
				bonesChangeCounter = 0.0f;	
			}*/


			/*for (auto& finalTransform : characterModel.GetBoneNameToInfo())
			{
				characterShader.SetUniformMatrix4fv()
			}*/




			characterShader.SetUniformMatrix4fv("model", modelMatrix);
			renderer.DrawModel(characterModel, characterShader);




			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}