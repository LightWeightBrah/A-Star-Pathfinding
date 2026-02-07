#include "Entity.h"

Entity::Entity(glm::vec3 startPosition, const std::string& filepath) :
	position(startPosition), startPoisiton(startPosition)
{
	resources = ResourceManager::GetModelData(filepath);
	animator  = Animator  (&resources->model);
	animator.PlayAnimation(&resources->animations.at(AnimationType::IDLE));
}

void Entity::SetPath(std::vector<GridPoint> gridPoints)
{
	for (auto& point : gridPoints)
		movingPoints.push({ point.x, FOOT_Y_HEIGHT, point.z });
}

void Entity::Update(float deltaTime)
{
	UpdateAnimations(deltaTime);

	if (movingPoints.empty())
		return;

	glm::vec3 target = movingPoints.front();
	glm::vec3 direction = target - position;

	float distanceToTarget = glm::length(direction);

	if (distanceToTarget < 0.1f)
	{
		position = target;
		movingPoints.pop();
	}
	else
	{
		glm::vec3 moveDirection = glm::normalize(direction);
		position += moveDirection * MOVE_SPEED * deltaTime;
	}
}

void Entity::Reset()
{
	position	 = startPoisiton;
	movingPoints = std::queue<glm::vec3>();
}

glm::mat4 Entity::GetModelMatrix()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(0.00035f, 0.00035f, 0.00035f));

	return model;
}

void Entity::UpdateAnimations(float deltaTime)
{
	bool isMoving = !movingPoints.empty();
	AnimationType newAnimationType = isMoving ? AnimationType::RUNNING : AnimationType::IDLE;

	Animation* animationtoPlay = &resources->animations.at(newAnimationType);
	if(animator.GetCurrentAnimation() != animationtoPlay)
		animator.PlayAnimation(animationtoPlay);

	animator.UpdateAnimation(deltaTime);
}