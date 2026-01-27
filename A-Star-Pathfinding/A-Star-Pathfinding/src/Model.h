#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

class Model 
{
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<TextureItem> texturesLoaded;

private:
	void LoadModel(std::string path, bool flipUV);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TextureItem> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
	Model(const std::string& path, bool flipUV);

	inline const std::vector<Mesh>& GetMeshes() const { return meshes; }
};