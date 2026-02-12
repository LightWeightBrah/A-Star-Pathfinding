#include "Entity.h"
#include "Mesh.h"

Entity::Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
	: mesh(std::move(mesh)), material(std::move(material))
{

}

const unsigned int Entity::GetIndicesCount() const { return static_cast<unsigned int>(mesh->GetIndexCount()); }

Entity& Entity::SetPosition(const glm::vec3& position)
{
	transform.position = position;
	isDirty = true;
	return *this;
}

Entity& Entity::SetRotation(const glm::vec3& rotation)
{
	transform.rotation = rotation;
	isDirty = true;
	return *this;
}

Entity& Entity::SetScale(const glm::vec3& scale)
{
	transform.scale = scale;
	isDirty = true;
	return *this;
}

Entity& Entity::SetMesh(std::shared_ptr<Mesh> mesh)
{
	this->mesh = std::move(mesh);
	return *this;
}

Entity& Entity::SetMaterial(std::shared_ptr<Material> material)
{
	this->material = std::move(material);
	return *this;
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