#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <map>

struct BoneInfo
{
	unsigned int boneId = 0;
	glm::mat4	 offset;
	glm::mat4	 finalTransformation;

	BoneInfo() {};
	BoneInfo(glm::mat4& offset) : offset(offset), finalTransformation(0.0f) {};
};

class Model 
{
private:
	std::vector<Mesh>				meshes;
	std::string						directory;
	std::vector<TextureItem>		texturesLoaded;

	std::map<std::string, BoneInfo> boneNameToInfo;
	int totalBones = 0;

private:
	void LoadModel		(std::string path, bool flipUV);
	void ProcessNode	(aiNode* node, const aiScene* scene, aiMatrix4x4& parentTransform);
	Mesh ProcessMesh	(aiMesh* mesh, int meshIndex, const aiScene* scene);

	void ProcessMeshBones		(aiMesh* mesh, std::vector<Vertex>& vertices, int meshIndex);
	void ProcessMeshSingleBone	(aiMesh* mesh, std::vector<Vertex>& vertices, int meshIndex, int boneIndex);

	void SetBonesForVertex(std::vector<Vertex>& vertices, unsigned int vertexId, unsigned int boneId, float weight);

	unsigned int GetBoneId		(aiBone* bone);



	std::vector<TextureItem> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
	Model(const std::string& path, bool flipUV);

	inline const std::vector<Mesh>& GetMeshes() const { return meshes; }
	inline const int GetTotalBones()			const { return totalBones; }
	inline const std::map<std::string, BoneInfo> GetBoneNameToInfo() const{ return boneNameToInfo; }

};