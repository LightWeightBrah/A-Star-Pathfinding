#pragma once

#include <iostream>
#include <GL/glew.h>

#include "Mesh.h"
#include "Shader.h"
#include "BufferLayout.h"
#include "VertexArray.h"
#include "Texture.h"

Mesh::Mesh(const void* vertexData, unsigned int dataSize, std::vector<unsigned int> indices,
		std::vector<TextureItem> textures, const BufferLayout& bufferLayout)
	: indices(indices), textures(textures)
{
	VAO = std::make_unique<VertexArray>();
	VBO = std::make_unique<VertexBuffer>(vertexData, dataSize);
	EBO = std::make_unique<ElementBuffer>(&indices[0], indices.size() * sizeof(unsigned int));

	VAO->AddBuffer(*VBO, bufferLayout);
}

void Mesh::BindTextures(const Shader& shader) const
{
	unsigned int diffuseNr  = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		textures[i].texture->Bind(i);

		std::string name   = textures[i].type;
		std::string number = (name == "texture_diffuse") ? std::to_string(diffuseNr++) : std::to_string(specularNr++);

		shader.SetUniform1i(name + number, i);
	}
}