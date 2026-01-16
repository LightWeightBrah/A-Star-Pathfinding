#include "ElementBuffer.h"

#include "Renderer.h"

ElementBuffer::ElementBuffer(const unsigned int* data, unsigned int count) 
	: count(count)
{
	GLCall(glGenBuffers(1, &rendererId));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

ElementBuffer::~ElementBuffer()
{
	GLCall(glDeleteBuffers(1, &rendererId));
}

void ElementBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
}

void ElementBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
