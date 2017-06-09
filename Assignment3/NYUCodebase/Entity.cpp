#include "Entity.h"

Entity::Entity()
{
	size = Vector3(1, 1, 1);
}

Entity::~Entity()
{
}

void Entity::Update(float elapsed)
{
	position.x += velocity.x * elapsed;
	position.y += velocity.y * elapsed;
	position.z += velocity.z * elapsed;
}

void Entity::Draw(ShaderProgram *program) {
	modelMatrix.setPosition(position.x, position.y, position.z);
	//modelMatrix.setScale(size.x, size.y, size.z);

	program->setProjectionMatrix(*projectionMatrix);
	program->setModelMatrix(modelMatrix);
	program->setViewMatrix(*viewMatrix);

	sprite->Draw(program);
}

bool Entity::isCollidingWith(Entity *other) {
	if (position.y - size.y / 2 > other->position.y + other->size.y / 2 ||
		position.y + size.y / 2 < other->position.y - other->size.y / 2 ||
		position.x - size.x / 2 > other->position.x + other->size.x / 2 ||
		position.x + size.x / 2 < other->position.x - other->size.x / 2) {
		return false;
	}
	return true;
}