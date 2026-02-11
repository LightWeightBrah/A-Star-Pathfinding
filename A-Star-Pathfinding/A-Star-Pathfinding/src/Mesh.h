#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Config.h"

class BufferLayout;

struct StaticVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct SkinnedVertex : StaticVertex
{
	unsigned int boneIDs[MAX_NUM_BONES_PER_VERTEX];
	float		 weights[MAX_NUM_BONES_PER_VERTEX];
};

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