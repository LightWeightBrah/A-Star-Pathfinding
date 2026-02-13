#pragma once

#include <memory>
#include <glm/glm.hpp>

class Shader;

class LightSource
{
public:
	struct Data
	{
		glm::vec3		position				= glm::vec3(4.0f);

		glm::vec3		ambientIntensity		= glm::vec3(0.2f);
		glm::vec3		diffuseIntensity		= glm::vec3(0.5f);
		glm::vec3		specularIntensity		= glm::vec3(1.0f);

	};

	LightSource(std::shared_ptr<Shader> shader);

	LightSource& SetAmbient(const glm::vec3& ambient);
	LightSource& SetDiffuse(const glm::vec3& diffuse);
	LightSource& SetSpecular(const glm::vec3& specular);
	LightSource& SetPosition(const glm::vec3& position);

	LightSource& Apply(Shader& shaderToLightUp);

	inline const Data& GetData()			   const { return data; }
	inline std::shared_ptr<Shader> GetShader() const { return shader; }

private:
	std::shared_ptr<Shader>		shader;
	Data						data;
};