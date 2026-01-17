#pragma once
#include <GL/glew.h>

#include "VertexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Texture.h"

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef _DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Clear(float r, float g, float b, float a) const;
	void Draw(const VertexArray& VAO, const ElementBuffer& EBO, const Shader& shader) const;
};