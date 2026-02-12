#include "Entity.h"

Entity::Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
	: mesh(std::move(mesh)), material(std::move(material))
{

}

void Entity::SetPosition(const glm::vec3& position)
{
	transform.position = position;
	isDirty = true;
}

const glm::mat4& Entity::GetModelMatrix()
{
	if (isDirty)
	{
		cachedModelMatrix = transform.GetModelMatrix();
		isDirty = false;
	}

	return cachedModelMatrix;
}