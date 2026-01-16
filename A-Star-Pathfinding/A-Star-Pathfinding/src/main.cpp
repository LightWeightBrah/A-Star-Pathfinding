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
#include "VertexBuffer.h"
#include "ElementBuffer.h"

#include "GL/glew.h"
#include <GLFW/glfw3.h>


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
	//positions				//colors
	 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f
};

unsigned int indicies[] = {
	0, 1, 2, //first triangle
	2, 3, 0  //2nd triangle
};



struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType 
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return  { ss[0].str(), ss[1].str()};

}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "ERROR: COULDN'T COMPILE " <<
			(type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " SHADER: " << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int CreateProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(program, length, &length, message);
		std::cout << "ERROR: LINKING SHADERS TO PROGRAM FAILED: " << message << std::endl;
		return 0;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	{

		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
		unsigned int program = CreateProgram(source.vertexSource, source.fragmentSource);

		unsigned int VAO;
		GLCall(glGenVertexArrays(1, &VAO));
		GLCall(glBindVertexArray(VAO));

		VertexBuffer VBO(verticies, sizeof(verticies));

		ElementBuffer EBO(indicies, sizeof(indicies));

		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glEnableVertexAttribArray(1));

		GLCall(glBindVertexArray(0));
		VBO.Unbind();
		EBO.Unbind();

		int nrAttributes;
		GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes));
		std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;


		while (!glfwWindowShouldClose(window))
		{
			processInput(window);

			GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			GLCall(glUseProgram(program));

			/*float timeValue = glfwGetTime();
			float greenValue = sin(timeValue) / 2.0f + 0.5f;
			int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
			glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/

			GLCall(glBindVertexArray(VAO));
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		GLCall(glDeleteVertexArrays(1, &VAO));
		GLCall(glDeleteProgram(program));
	}

	glfwTerminate();
	return 0;
}