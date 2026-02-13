#include "LightSource.h"
#include "Shader.h"

LightSource::LightSource(
	std::shared_ptr<Shader> shader)
	:
	shader(std::move(shader))
{

}

LightSource& LightSource::Apply(Shader& shaderToLightUp)
{
	shaderToLightUp.Bind();
	shaderToLightUp.SetUniform3f("lightSource.ambientIntensity",	data.ambientIntensity);
	shaderToLightUp.SetUniform3f("lightSource.diffuseIntensity",	data.diffuseIntensity);
	shaderToLightUp.SetUniform3f("lightSource.specularIntensity",	data.specularIntensity);
	shaderToLightUp.SetUniform3f("lightSource.position",			data.position);

	return *this;
}

LightSource& LightSource::SetAmbient(const glm::vec3& ambient)
{
	data.ambientIntensity = ambient;
	return *this;
}

LightSource& LightSource::SetDiffuse(const glm::vec3& diffuse)
{
	data.diffuseIntensity = diffuse;
	return *this;
}

LightSource& LightSource::SetSpecular(const glm::vec3& specular)
{
	data.specularIntensity = specular;
	return *this;
}

LightSource& LightSource::SetPosition(const glm::vec3& position)
{
	data.position = position;
	return *this;
}


