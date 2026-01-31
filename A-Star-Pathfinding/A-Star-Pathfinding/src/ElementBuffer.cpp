#include "Renderer.h"

ElementBuffer::ElementBuffer(const unsigned int* data, unsigned int size) 
{
	count = size / sizeof(unsigned int);

	GLCall(glGenBuffers(1, &boneId));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boneId));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

ElementBuffer::~ElementBuffer()
{
	GLCall(glDeleteBuffers(1, &boneId));
}

void ElementBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boneId));
}

void ElementBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}