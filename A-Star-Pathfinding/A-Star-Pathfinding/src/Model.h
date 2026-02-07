#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <map>

struct BoneInfo
{
	unsigned int id = 0;
	glm::mat4	 offset;
};

class Model 
{
private:
	const aiScene*					scene;
	std::string						directory;

	std::vector<Mesh>				meshes;
	std::vector<TextureItem>		texturesLoaded;

	std::map<std::string, BoneInfo> boneNameToInfo;
	glm::mat4						globalInverseTransform;

	bool							hasAnimations;

private:
	void LoadModel(std::string path, bool flipUV);
	
	void ProcessNode(aiNode* node);
	Mesh ProcessMesh(aiMesh* mesh);

	void ProcessMeshBones		(aiMesh* mesh, std::vector<Vertex>& vertices);
	void ProcessMeshSingleBone	(aiMesh* mesh, std::vector<Vertex>& vertices, int boneIndex);

	void SetBonesForVertex		(std::vector<Vertex>& vertices, unsigned int vertexId, unsigned int id, float weight);

	unsigned int GetBoneId		(aiBone* bone);

	std::vector<TextureItem> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
	Model() = default;
	Model(const std::string& path, bool flipUV);

	inline const bool HasAnimations()			const { return hasAnimations; }
	inline const std::vector<Mesh>& GetMeshes() const { return meshes; }
	
	inline const std::map<std::string, BoneInfo> GetBoneNameToInfo() const { return boneNameToInfo; }
	inline const glm::mat4				 GetGlobalInverseTransform() const { return globalInverseTransform; }

};