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
#include "external/stb_image.h"

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
	 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	//top right
	 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	//bottom right
    -0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	//bottom left
	-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,   0.0f, 1.0f	//top left
};

unsigned int indicies[] = {
	0, 1, 2, //first triangle
	2, 3, 0  //2nd triangle
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

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	{
		Shader shader("res/shaders/Basic.shader");

		VertexArray VAO;
		VertexBuffer VBO(verticies, sizeof(verticies));
		ElementBuffer EBO(indicies, sizeof(indicies));

		VAO.AddAttrib(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
		VAO.AddAttrib(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		VAO.AddAttrib(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));


		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* data = stbi_load("res/Textures/container.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "ERROR: COULDN'T LOAD TEXTURE" << std::endl;
		}
		stbi_image_free(data);

		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();
		shader.Unbind();

		Renderer renderer;
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);

			renderer.Clear(0.2f, 0.3f, 0.3f, 1.0f);

			glBindTexture(GL_TEXTURE_2D, texture);
			renderer.Draw(VAO, EBO, shader);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}