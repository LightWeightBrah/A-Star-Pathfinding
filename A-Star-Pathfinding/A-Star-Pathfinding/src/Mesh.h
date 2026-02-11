#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Config.h"

class Texture;
class Shader;

class VertexArray;
class VertexBuffer;
class ElementBuffer;

class BufferLayout;

struct TextureItem 
{
	std::shared_ptr<Texture> texture;
	std::string				 type;
};

class Mesh	
{
private:
	std::vector<unsigned int>		indices;
	std::vector<TextureItem>		textures;

	std::unique_ptr<VertexArray>	VAO;
	std::unique_ptr<VertexBuffer>	VBO;
	std::unique_ptr<ElementBuffer>	EBO;

public:
	Mesh(const void* vertexData, unsigned int dataSize, std::vector<unsigned int> indices, 
		std::vector<TextureItem> textures, const BufferLayout& bufferLayout);

	void BindTextures(const Shader& shader) const;

	inline const VertexArray& GetVAO()		const { return *VAO; }
	inline const ElementBuffer& GetEBO()	const { return *EBO; }
	inline unsigned int GetIndexCount()		const { return (unsigned int)indices.size(); }
};