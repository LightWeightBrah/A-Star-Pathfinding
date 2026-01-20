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

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

glm::vec3 cameraPos		=	glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront	=	glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp		=	glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = WINDOW_WIDTH / 2;
float lastY = WINDOW_HEIGHT / 2;

bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void OnWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void HandleInput(GLFWwindow* window)
{
	const float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.HandleKeyboard(Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.HandleKeyboard(Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.HandleKeyboard(Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.HandleKeyboard(Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.HandleKeyboard(Movement::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.HandleKeyboard(Movement::DOWN, deltaTime);

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
	FindPathInConsole();

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

	GLCall(glEnable(GL_DEPTH_TEST));
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, OnWindowResized);
	glfwSetCursorPosCallback(window, OnMouse);
	glfwSetScrollCallback(window, OnScroll);
	{
		Shader shader("res/shaders/Basic.shader");

		VertexArray VAO;
		VertexBuffer VBO(verticies, sizeof(verticies));
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

		Renderer renderer;

		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			HandleInput(window);

			renderer.Clear(0.2f, 0.3f, 0.3f, 1.0f);

			shader.Bind();
			texture1.Bind();
			texture2.Bind(1);

			glm::mat4 projection = glm::perspective(glm::radians(camera.GetFov()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
			shader.SetUniformMatrix4fv("projection", projection);

			glm::mat4 view = camera.GetViewMatrix();
			shader.SetUniformMatrix4fv("view", view);

			for (int i = 0; i < 2; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(1.5f * i, 1.5f * i, 0.0f));
				shader.SetUniformMatrix4fv("model", model);
				renderer.Draw(VAO, EBO, shader);
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}