#include <utility>

#include "Material.h"
#include "Shader.h"

Material::Material(
	std::shared_ptr<Shader> shader,
	const glm::vec3&		ambient,
	const glm::vec3&		diffuse,
	const glm::vec3&		specular,
	float					shiny
)
	: shader(std::move(shader))
	, ambientColor(ambient)
	, diffuseColor(diffuse)
	, specularColor(specular)
	, shininess(shiny)
{

}

void Material::Apply()
{
	if (!shader)
		return;

	shader->Bind();
	shader->SetUniform3f("material.ambientColor",	ambientColor);
	shader->SetUniform3f("material.diffuseColor",	diffuseColor);
	shader->SetUniform3f("material.specularColor",	specularColor);
	shader->SetUniform1f("material.shininess",		shininess);
}