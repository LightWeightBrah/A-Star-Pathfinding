#pragma once
#include "Model.h"
#include <iostream>
#include "Texture.h"
#include "Mesh.h"

Model::Model(const std::string& path) 
{
	LoadModel(path);
}

void Model::LoadModel(std::string path) 
{
	std::cout << "Loading model: " << path << std::endl;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

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
		meshes.emplace_back(ProcessMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) 
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureItem> mesh_textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex v;
		v.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		v.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		v.TexCoords = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);
		vertices.push_back(v);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices.push_back(mesh->mFaces[i].mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<TextureItem> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		mesh_textures.insert(mesh_textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<TextureItem> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		mesh_textures.insert(mesh_textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, mesh_textures);
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
			texture.type = typeName;

			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}
	return textures;
}