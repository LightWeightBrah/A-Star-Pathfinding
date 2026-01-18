#include <iostream>
#include "Renderer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "OPEN_GL ERROR (" << error << "): " << function << " "
			<< file << ":" << line << std::endl;
		return false;
	}

	return true;
}

void Renderer::Clear(float r, float g, float b, float a) const
{
	GLCall(glClearColor(r, g,b, a));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& VAO, const ElementBuffer& EBO, const Shader& shader) const
{
	shader.Bind();
	VAO.Bind();
	EBO.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, EBO.GetCount(), GL_UNSIGNED_INT, 0));
}
