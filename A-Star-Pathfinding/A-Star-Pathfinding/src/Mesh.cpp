#pragma once

#include <iostream>
#include <GL/glew.h>

#include "Mesh.h"
#include "Shader.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureItem> textures)
: vertices(vertices), indices(indices), textures(textures)
{
	SetupMesh();
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

void Mesh::SetupMesh() 
{
	VAO = std::make_unique<VertexArray>();
	VBO = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));
	EBO = std::make_unique<ElementBuffer>(&indices[0], indices.size()  * sizeof(unsigned int));

	VAO->AddAttrib(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
	VAO->AddAttrib(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	VAO->AddAttrib(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
}