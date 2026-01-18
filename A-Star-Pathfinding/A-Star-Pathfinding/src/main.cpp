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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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

	GLCall(glEnable(GL_DEPTH_TEST));

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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

			//glm::mat4 trans = glm::mat4(1.0f);
			//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
			//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 0.0f));
			glm::mat4 view = glm::mat4(1.0f);
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
			glm::mat4 projection;
			projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

			shader.Bind();
			texture1.Bind();
			texture2.Bind(1);

			shader.SetUniformMatrix4fv("model", 1, false, glm::value_ptr(model));
			shader.SetUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
			shader.SetUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));

			renderer.Draw(VAO, EBO, shader);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}