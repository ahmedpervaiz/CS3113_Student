#include "Entity.h"

Entity::Entity()
{
	size = Vector3(1, 1, 1);
}

Entity::~Entity()
{
}

float lerp(float v0, float v1, float t) {
	return (1.0f - t) * v0 + t * v1;
}

void Entity::Update(float elapsed)
{
	if (!isStatic) {
		velocity.x = lerp(velocity.x, 0.0f, elapsed * friction_x);
		velocity.y = lerp(velocity.y, 0.0f, elapsed * friction_y);

		velocity.x += (acceleration.x + gravity_x) * elapsed;
		velocity.y += (acceleration.y + gravity_y) * elapsed;
		velocity.z += acceleration.z * elapsed;

		position.z += velocity.z * elapsed;

		collidedBottom = false;
		collidedTop = false;
		collidedLeft = false;
		collidedRight = false;
	}
}

void Entity::ApplyY(float elapsed)
{
	if (!isStatic) {
		position.y += velocity.y * elapsed;
	}
}

void Entity::ApplyX(float elapsed)
{
	if (!isStatic) {
		position.x += velocity.x * elapsed;
	}
}

void Entity::Draw(ShaderProgram *program) {
	Matrix modelMatrix = Matrix();
	modelMatrix.setPosition(position.x, position.y, position.z);
	//modelMatrix.setScale(size.x, size.y, size.z);

	program->setModelMatrix(modelMatrix);

	sprite->Draw(program);
}

bool Entity::isCollidingWith(const Entity & other)
{
	if (position.y - size.y / 2 > other.position.y + other.size.y / 2 ||
		position.y + size.y / 2 < other.position.y - other.size.y / 2 ||
		position.x - size.x / 2 > other.position.x + other.size.x / 2 ||
		position.x + size.x / 2 < other.position.x - other.size.x / 2) {
		return false;
	}
	return true;
}

void Entity::checkBox(float u, float v, float width, float height) {
	if (position.y - size.y / 2 > v + height / 2 ||
		position.y + size.y / 2 < v - height / 2 ||
		position.x - size.x / 2 > u + width / 2 ||
		position.x + size.x / 2 < u - width / 2) {
		return;
	}

}