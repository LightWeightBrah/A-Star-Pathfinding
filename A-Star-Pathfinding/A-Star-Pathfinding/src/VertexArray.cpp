#include <GL/glew.h>
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &id));
	Bind();
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &id));
}

void VertexArray::AddAttrib(unsigned int index, unsigned int size, 
	unsigned int type, bool normalized, unsigned int stride, const void* offset) const
{
	GLCall(glVertexAttribPointer(index, size, type, 
		normalized ? GL_TRUE : GL_FALSE, stride, (void*)offset));
	GLCall(glEnableVertexAttribArray(index));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(id));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
