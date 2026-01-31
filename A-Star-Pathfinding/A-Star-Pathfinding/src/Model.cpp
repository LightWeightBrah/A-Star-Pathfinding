#pragma once
#include "Model.h"
#include <iostream>
#include "Texture.h"
#include "Mesh.h"

Model::Model(const std::string& path, bool flipUV)
{
	LoadModel(path, flipUV);
}

void Model::LoadModel(std::string path, bool flipUV) 
{
	std::cout << "Loading model: " << path << std::endl;
	Assimp::Importer importer;
	const aiScene* scene;

	if (flipUV)
	{
		#define ASSIMP_LOAD_FLAGS aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals
	}
	else
	{
		#define ASSIMP_LOAD_FLAGS aiProcess_Triangulate | aiProcess_GenNormals
	}
	scene = importer.ReadFile(path, ASSIMP_LOAD_FLAGS);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('\\/'));
	ProcessNode(scene->mRootNode, scene);

	std::cout << "Model loaded successfully!" << std::endl;

}

void Model::ProcessNode(aiNode* node, const aiScene* scene) 
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) 
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(ProcessMesh(mesh, i, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) 
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, int meshIndex, const aiScene* scene)
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
		ProcessMeshBones(mesh, vertices, meshIndex);

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

void Model::ProcessMeshBones(aiMesh* mesh, std::vector<Vertex>& vertices, int meshIndex)
{
	for (int i = 0; i < mesh->mNumBones; i++)
		ProcessMeshSingleBone(mesh, vertices, meshIndex, i);
}

void Model::ProcessMeshSingleBone(aiMesh* mesh, std::vector<Vertex>& vertices, int meshIndex, int boneIndex)
{
	aiBone* bone = mesh->mBones[boneIndex];
	totalBones++;

	std::cout << "\nBone: " << boneIndex << " " << bone->mName.C_Str()
		<< "\nNumber of vertices affected by this bone: " << bone->mNumWeights << std::endl;

	
	for (int i = 0; i < bone->mNumWeights; i++)
	{
		aiVertexWeight& vertexWeight = bone->mWeights[i];

		unsigned int vertexId = vertexWeight.mVertexId;
		unsigned int boneId   = GetBoneId(bone);
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

	if (boneNameToIndex.find(boneName) == boneNameToIndex.end())
	{
		boneId					  = boneNameToIndex.size();
		boneNameToIndex[boneName] = boneId;
	}
	else
	{
		boneId = boneNameToIndex[boneName];
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