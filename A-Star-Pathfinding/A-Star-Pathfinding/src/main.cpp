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

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float pitch = 0.0f;
float yaw = -90.0f;
float lastX = WINDOW_WIDTH / 2;
float lastY = WINDOW_HEIGHT / 2;
bool firstMouse = true;

float fov = 45.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	const float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraFront * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraFront * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraPos += cameraUp * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraPos -= cameraUp * cameraSpeed;

}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	fov -= (float)yOffset;
	
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
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

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
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

		//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
		//glm::mat4 trans = glm::mat4(1.0f);
		//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
		//vec = trans * vec;
		//std::cout << vec.x << vec.y << vec.z << std::endl;

		



		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();
		shader.Unbind();

		Renderer renderer;

		while (!glfwWindowShouldClose(window))
		{
			processInput(window);

			renderer.Clear(0.2f, 0.3f, 0.3f, 1.0f);

			shader.Bind();
			texture1.Bind();
			texture2.Bind(1);

			//glm::mat4 trans = glm::mat4(1.0f);
			//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
			//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));


			

			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 0.0f));
			shader.SetUniformMatrix4fv("model", 1, false, glm::value_ptr(model));
			renderer.Draw(VAO, EBO, shader);
			model = glm::translate(model, glm::vec3(1.5f, 1.5f, 0.0f));
			shader.SetUniformMatrix4fv("model", 1, false, glm::value_ptr(model));
			renderer.Draw(VAO, EBO, shader);



			glm::mat4 view = glm::mat4(1.0f);
			//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
			//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
			//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
			//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
			//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

			//const float radius = 10.0f;
			//float camX = sin(glfwGetTime()) * radius;
			//float camZ = cos(glfwGetTime()) * radius;
			//					//camera positon			//target position			//up vector in world space
			//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			
			//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			


			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


			glm::mat4 projection = glm::mat4(1.0f);
			projection = glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
			shader.SetUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
			shader.SetUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));


			/*for (int y = 0; y < 20; y++)
			{
				for (int x = 0; x < 20; x++)
				{
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3((float)x - 10.0f, (float)y - 10.0f, -5.0f));
					model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 0.0f));
					shader.SetUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

					renderer.Draw(VAO, EBO, shader);
				}
			}*/

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}