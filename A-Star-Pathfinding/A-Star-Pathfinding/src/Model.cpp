#pragma once
#include <iostream>
#include <glm/gtx/quaternion.hpp>

#include "Model.h"
#include "Texture.h"
#include "Mesh.h"
#include "AssimpUtilities.h"


Model::Model(const std::string& path, bool flipUV)
{
	LoadModel(path, flipUV);
}

void Model::LoadModel(std::string path, bool flipUV) 
{
	std::cout << "Loading model: " << path << std::endl;

	unsigned int flags = aiProcess_Triangulate | aiProcess_GenNormals;
	if (flipUV)
		flags |= aiProcess_FlipUVs;

	scene = importer.ReadFile(path, flags);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of("\\ / "));

	globalInverseTransform = glm::inverse
	(AssimpUtilities::ConvertAssimpMatrixToGLM(scene->mRootNode->mTransformation));

	ProcessNode(scene->mRootNode);

	std::cout << "Model loaded successfully!" << std::endl;

}

void Model::ProcessNode(aiNode* node) 
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) 
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(ProcessMesh(mesh));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) 
	{
		ProcessNode(node->mChildren[i]);
	}
}

void Model::PlayAnimation(float time)
{
	if (!scene || !scene->HasAnimations()) 
		return;

	float TicksPerSecond = (float)(scene->mAnimations[0]->mTicksPerSecond != 0 ?
		scene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float TimeInTicks = time * TicksPerSecond;
	float AnimationTimeTicks = fmod(TimeInTicks, (float)scene->mAnimations[0]->mDuration);

	UpdateBoneMatrices(AnimationTimeTicks, scene->mRootNode, glm::mat4(1.0f));
}

void Model::UpdateBoneMatrices(float animTimeTicks, aiNode* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->mName.data;
	glm::mat4 nodeTransform = AssimpUtilities::ConvertAssimpMatrixToGLM(node->mTransformation);

	if (scene->HasAnimations())
	{
		aiAnimation* anim = scene->mAnimations[0];
		const aiNodeAnim* nodeAnim = FindNodeAnim(anim, nodeName);

		if (nodeAnim)
		{
			glm::vec3 scale = InterpolateScaling(animTimeTicks, nodeAnim);
			glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);

			glm::quat rotation = InterpolateRotation(animTimeTicks, nodeAnim);
			glm::mat4 rotationMat = glm::toMat4(rotation);

			glm::vec3 translation = InterpolatePosition(animTimeTicks, nodeAnim);
			glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), translation);

			nodeTransform = translationMat * rotationMat * scaleMat;
		}
	}

	glm::mat4 globalTransform = parentTransform * nodeTransform;

	if (boneNameToInfo.find(nodeName) != boneNameToInfo.end())
	{
		boneNameToInfo[nodeName].finalTransformation =
			globalInverseTransform * globalTransform * boneNameToInfo[nodeName].offset;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		UpdateBoneMatrices(animTimeTicks, node->mChildren[i], globalTransform);
	}
}

template<typename T>
unsigned int Model::GetKeyIndex(float animTime, const T* keys, unsigned int numKeys)
{
	//we are searching for frame that is before the current animation time
	for (unsigned int i = 0; i < numKeys - 1; i++)
	{
		if (animTime < (float)keys[i + 1].mTime)
			return i;
	}
	return 0;
}

float Model::CalculateFactor(float animTime, float frameTime, float nextFrameTime)
{
	float deltaTime = nextFrameTime - frameTime;
	float factor	= (animTime - frameTime) / deltaTime;
	return factor;
}

glm::vec3 Model::InterpolatePosition(float animTime, const aiNodeAnim* nodeAnim)
{
	if (nodeAnim->mNumPositionKeys == 1)
		return glm::vec3(nodeAnim->mPositionKeys[0].mValue.x, nodeAnim->mPositionKeys[0].mValue.y, nodeAnim->mPositionKeys[0].mValue.z);

	unsigned int index = GetKeyIndex(animTime, nodeAnim->mPositionKeys, nodeAnim->mNumPositionKeys);
	unsigned int nextIndex = index + 1;

	float factor = CalculateFactor(animTime, (float)nodeAnim->mPositionKeys[index].mTime, (float)nodeAnim->mPositionKeys[nextIndex].mTime);

	const aiVector3D& start = nodeAnim->mPositionKeys[index].mValue;
	const aiVector3D& end	= nodeAnim->mPositionKeys[nextIndex].mValue;
	aiVector3D result = start + factor * (end - start);

	return glm::vec3(result.x, result.y, result.z);
}

glm::vec3 Model::InterpolateScaling(float animTime, const aiNodeAnim* nodeAnim)
{
	if (nodeAnim->mNumScalingKeys == 1)
		return glm::vec3(nodeAnim->mScalingKeys[0].mValue.x, nodeAnim->mScalingKeys[0].mValue.y, nodeAnim->mScalingKeys[0].mValue.z);

	unsigned int index = GetKeyIndex(animTime, nodeAnim->mScalingKeys, nodeAnim->mNumScalingKeys);
	unsigned int nextIndex = index + 1;

	float factor = CalculateFactor(animTime, (float)nodeAnim->mScalingKeys[index].mTime, (float)nodeAnim->mScalingKeys[nextIndex].mTime);

	const aiVector3D& start = nodeAnim->mScalingKeys[index].mValue;
	const aiVector3D& end	= nodeAnim->mScalingKeys[nextIndex].mValue;
	aiVector3D result = start + factor * (end - start);

	return glm::vec3(result.x, result.y, result.z);
}

glm::quat Model::InterpolateRotation(float animTime, const aiNodeAnim* nodeAnim)
{
	if (nodeAnim->mNumRotationKeys == 1)
	{
		auto q = nodeAnim->mRotationKeys[0].mValue;
		return glm::quat(q.w, q.x, q.y, q.z);
	}

	unsigned int index = GetKeyIndex(animTime, nodeAnim->mRotationKeys, nodeAnim->mNumRotationKeys);
	unsigned int nextIndex = index + 1;

	float factor = CalculateFactor(animTime, (float)nodeAnim->mRotationKeys[index].mTime, (float)nodeAnim->mRotationKeys[nextIndex].mTime);

	const aiQuaternion& start = nodeAnim->mRotationKeys[index].mValue;
	const aiQuaternion& end	  = nodeAnim->mRotationKeys[nextIndex].mValue;

	aiQuaternion out;
	aiQuaternion::Interpolate(out, start, end, factor);
	out = out.Normalize();

	return glm::quat(out.w, out.x, out.y, out.z);
}

const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* animation, std::string& nodeName)
{
	for (unsigned int i = 0; i < animation->mNumChannels; i++) 
	{
		const aiNodeAnim* nodeAnim = animation->mChannels[i];
		
		if (std::string(nodeAnim->mNodeName.data) == nodeName) 
			return nodeAnim;
	}
	return nullptr;
}

Mesh Model::ProcessMesh(aiMesh* mesh)
{
	std::vector<Vertex>		  vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureItem>  meshTextures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex v;
		v.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		v.Normal   = { mesh->mNormals[i].x , mesh->mNormals[i].y , mesh->mNormals[i].z  };
		v.TexCoords = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);
		for (unsigned int j = 0; j < MAX_NUM_BONES_PER_VERTEX; j++)
		{
			v.boneIDs[j] = 0;
			v.weights[j] = 0.0f;
		}
		
		vertices.push_back(v);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices.push_back(mesh->mFaces[i].mIndices[j]);
	}

	if (mesh->HasBones())
		ProcessMeshBones(mesh, vertices);

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<TextureItem> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		meshTextures.insert(meshTextures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<TextureItem> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		meshTextures.insert(meshTextures.end(), specularMaps.begin(), specularMaps.end());
	}

	std::cout << "\nMesh: " << mesh->mName.C_Str() << "\nTotal number of Vertices : " << vertices.size()
		<< "\nTotal Indices: " << indices.size() << "\nTotal Bones: " << mesh->mNumBones << std::endl;

	return Mesh(vertices, indices, meshTextures);
}

void Model::ProcessMeshBones(aiMesh* mesh, std::vector<Vertex>& vertices)
{
	for (int i = 0; i < mesh->mNumBones; i++)
		ProcessMeshSingleBone(mesh, vertices, i);
}

void Model::ProcessMeshSingleBone(aiMesh* mesh, std::vector<Vertex>& vertices, int boneIndex)
{
	aiBone* bone		= mesh->mBones[boneIndex];
	unsigned int boneId = GetBoneId(bone);

	totalBones++;

	std::cout << "\nBone: " << boneIndex << " " << bone->mName.C_Str()
		<< "\nNumber of vertices affected by this bone: " << bone->mNumWeights << std::endl;

	
	for (int i = 0; i < bone->mNumWeights; i++)
	{
		aiVertexWeight& vertexWeight = bone->mWeights[i];

		unsigned int vertexId = vertexWeight.mVertexId;
		float        weight   = vertexWeight.mWeight;
		
		SetBonesForVertex(vertices, vertexId, boneId, weight);

		/*std::cout << "\nVertex id: " << vertexWeight.mVertexId << "\nWeight: " 
			<< vertexWeight.mWeight << std::endl;*/
	}
}

void Model::SetBonesForVertex(std::vector<Vertex>& vertices, unsigned int vertexId, unsigned int boneId, float weight)
{
	for (unsigned int i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++)
	{
		Vertex& vertex = vertices[vertexId];

		if (vertex.weights[i] == 0.0)
		{
			vertex.boneIDs[i] = boneId;
			vertex.weights[i] = weight;
			break;
		}
	}
}

unsigned int Model::GetBoneId(aiBone* bone)
{
	unsigned int boneId = 0;
	std::string boneName(bone->mName.C_Str());

	if (boneNameToInfo.find(boneName) == boneNameToInfo.end())
	{
		boneId							= boneNameToInfo.size();
		boneNameToInfo[boneName].boneId = boneId;
		boneNameToInfo[boneName].offset = AssimpUtilities::ConvertAssimpMatrixToGLM(bone->mOffsetMatrix);
	}
	else
	{
		boneId = boneNameToInfo[boneName].boneId;
	}

	return boneId;
}

std::vector<TextureItem> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<TextureItem> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) 
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		std::string fullPath = directory + "/" + str.C_Str();

		for (unsigned int j = 0; j < texturesLoaded.size(); j++) 
		{
			if (texturesLoaded[j].texture->GetFilepath() == fullPath) 
			{
				textures.push_back(texturesLoaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip) 
		{
			std::string fullPath = directory + "/" + str.C_Str();
			auto tex = std::make_shared<Texture>(fullPath);
			TextureItem texture;

			texture.texture = tex;
			texture.type	= typeName;

			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}
	return textures;
}