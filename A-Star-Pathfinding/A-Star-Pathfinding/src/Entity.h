#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Mesh;
class Material;

struct Transform
{
	glm::vec3 position		= glm::vec3(0.0f);
	glm::vec3 rotation		= glm::vec3(0.0f);
	glm::vec3 scale			= glm::vec3(1.0f);

	glm::mat4 GetModelMatrix() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);

		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::scale(model, scale);

		return model;
	}
};

class Entity
{
public:
	Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

	void SetPosition(const glm::vec3& position);
	
	const glm::mat4&				 GetModelMatrix();
	const std::shared_ptr<Mesh>&	 GetMesh()				const { return mesh; }
	const std::shared_ptr<Material>& GetMaterial()			const { return material; }

private:
	std::shared_ptr<Mesh>			 mesh;
	std::shared_ptr<Material>		 material;
									 
	Transform						 transform;
	glm::mat4						 cachedModelMatrix		= glm::mat4(1.0f);
	bool							 isDirty				= true;
};