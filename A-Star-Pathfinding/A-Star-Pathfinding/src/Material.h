#pragma once

#include <memory>
#include <glm/glm.hpp>

class Shader;

class Material
{
public:
	Material(
		std::shared_ptr<Shader> shader,
		const glm::vec3&		ambient			= glm::vec3(0.2f),
		const glm::vec3&		diffuse			= glm::vec3(0.5f),
		const glm::vec3&		specular		= glm::vec3(0.5f),
		float					shiny			= 32.0f
	);

	void Apply();

private:
	std::shared_ptr<Shader>		shader;

	glm::vec3					ambientColor;
	glm::vec3					diffuseColor;
	glm::vec3					specularColor;

	float						shininess;
};