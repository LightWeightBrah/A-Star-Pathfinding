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
	Assimp::Importer importer;
	const aiScene* scene;

	std::vector<Mesh>				meshes;
	std::string						directory;
	std::vector<TextureItem>		texturesLoaded;

	std::map<std::string, BoneInfo> boneNameToInfo;
	glm::mat4 globalInverseTransform;

	int totalBones = 0;

private:
	void LoadModel(std::string path, bool flipUV);
	
	void ProcessNode(aiNode* node);
	Mesh ProcessMesh(aiMesh* mesh);

	template<typename T>
	unsigned int GetKeyIndex	(float animTime, const T* keys, unsigned int numKeys);
	float		 CalculateFactor(float animTime, float frameTime, float nextFrameTime);

	glm::vec3 InterpolatePosition(float animTime, const aiNodeAnim* nodeAnim);
	glm::vec3 InterpolateScaling (float animTime, const aiNodeAnim* nodeAnim);
	glm::quat InterpolateRotation(float animTime, const aiNodeAnim* nodeAnim);

	void UpdateBoneMatrices		  (float time, aiNode* node, glm::mat4 parentTransform);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* animation, std::string& nodeName);

	void ProcessMeshBones		(aiMesh* mesh, std::vector<Vertex>& vertices);
	void ProcessMeshSingleBone	(aiMesh* mesh, std::vector<Vertex>& vertices, int boneIndex);

	void SetBonesForVertex(std::vector<Vertex>& vertices, unsigned int vertexId, unsigned int boneId, float weight);

	unsigned int GetBoneId		(aiBone* bone);



	std::vector<TextureItem> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
	Model(const std::string& path, bool flipUV);

	void PlayAnimation(float time);

	inline const std::vector<Mesh>& GetMeshes() const { return meshes; }
	inline const int GetTotalBones()			const { return totalBones; }
	inline const std::map<std::string, BoneInfo> GetBoneNameToInfo() const{ return boneNameToInfo; }

};