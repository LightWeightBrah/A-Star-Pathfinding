#pragma once

#include <memory>
#include <glm/glm.hpp>

class Shader;

class Material
{
public:
	struct Data
	{
		glm::vec3		ambientColor		= glm::vec3(0.2f);
		glm::vec3		diffuseColor		= glm::vec3(0.5f);
		glm::vec3		specularColor		= glm::vec3(0.5f);

		float			shininess			= 32.0f;
	};

	Material(std::shared_ptr<Shader> shader);

	Material& SetAmbient(const glm::vec3& color);
	Material& SetDiffuse(const glm::vec3& color);
	Material& SetSpecular(const glm::vec3& color);
	Material& SetShininess(float shininess);

	Material& Apply();

	inline const Data& GetData()			   const { return data; }
	inline std::shared_ptr<Shader> GetShader() const { return shader; }

private:
	std::shared_ptr<Shader>		shader;
	Data						data;
};