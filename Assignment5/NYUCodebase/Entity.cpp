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
	modelMatrix.setRotation(rotation);

	program->setModelMatrix(modelMatrix);

	sprite->Draw(program);
}

void Entity::CollisionFix(Entity & other)
{
	int maxChecks = 10;
	while (isCollidingWith(other) && maxChecks > 0) {
		Point responseVector = Point(other.position.x - position.x, other.position.y - position.y);

		responseVector.Normalize();

		position.x -= responseVector.x * 0.002f;
		position.y -= responseVector.y * 0.002f;
		velocity.x = 0;
		velocity.y = 0;
		other.position.x += responseVector.x * 0.002f;
		other.position.y += responseVector.y * 0.002f;
		other.velocity.x = 0;
		other.velocity.y = 0;

		maxChecks--;
	}
}

bool axisCalculation(Point normal, Point topleft, Point topright, Point bottomleft, Point bottomright,
	Point othertopleft, Point othertopright, Point otherbottomleft, Point otherbottomright, Vector3 position, Vector3 otherposition) {
	float centerDifference = fabs(Point(position).Dot(normal) - Point(otherposition).Dot(normal));

	float minAxis = topleft.Dot(normal);
	minAxis = fminf(minAxis, topright.Dot(normal));
	minAxis = fminf(minAxis, bottomleft.Dot(normal));
	minAxis = fminf(minAxis, bottomright.Dot(normal));

	float maxAxis = topleft.Dot(normal);
	maxAxis = fmaxf(maxAxis, topright.Dot(normal));
	maxAxis = fmaxf(maxAxis, bottomleft.Dot(normal));
	maxAxis = fmaxf(maxAxis, bottomright.Dot(normal));

	float otherminAxis = othertopleft.Dot(normal);
	otherminAxis = fminf(otherminAxis, othertopright.Dot(normal));
	otherminAxis = fminf(otherminAxis, otherbottomleft.Dot(normal));
	otherminAxis = fminf(otherminAxis, otherbottomright.Dot(normal));

	float othermaxAxis = othertopleft.Dot(normal);
	othermaxAxis = fmaxf(othermaxAxis, othertopright.Dot(normal));
	othermaxAxis = fmaxf(othermaxAxis, otherbottomleft.Dot(normal));
	othermaxAxis = fmaxf(othermaxAxis, otherbottomright.Dot(normal));

	return centerDifference - ((maxAxis - minAxis) + (othermaxAxis - otherminAxis)) / 2 >= 0;
}

bool Entity::isCollidingWith(const Entity & other)
{
	//for each axis of squares
	Point topleft = Point(position.x - size.x / 2, position.y + size.y / 2);
	Point topright = Point(position.x + size.x / 2, position.y + size.y / 2);
	Point bottomleft = Point(position.x - size.x / 2, position.y - size.y / 2);
	Point bottomright = Point(position.x + size.x / 2, position.y - size.y / 2);

	Point othertopleft = Point(other.position.x - other.size.x / 2, other.position.y + other.size.y / 2);
	Point othertopright = Point(other.position.x + other.size.x / 2, other.position.y + other.size.y / 2);
	Point otherbottomleft = Point(other.position.x - other.size.x / 2, other.position.y - other.size.y / 2);
	Point otherbottomright = Point(other.position.x + other.size.x / 2, other.position.y - other.size.y / 2);

	Point topedgenormal = (topleft - topright).Normal();
	Point leftedgenormal = (topleft - bottomleft).Normal();
	Point othertopedgenormal = (othertopleft - othertopright).Normal();
	Point otherleftedgenormal = (othertopleft - otherbottomleft).Normal();

	// Calculation
	if (axisCalculation(topedgenormal, topleft, topright, bottomleft, bottomright, othertopleft, othertopright, otherbottomleft, otherbottomright, position, other.position) ||
		axisCalculation(leftedgenormal, topleft, topright, bottomleft, bottomright, othertopleft, othertopright, otherbottomleft, otherbottomright, position, other.position) ||
		axisCalculation(othertopedgenormal, topleft, topright, bottomleft, bottomright, othertopleft, othertopright, otherbottomleft, otherbottomright, position, other.position) ||
		axisCalculation(otherleftedgenormal, topleft, topright, bottomleft, bottomright, othertopleft, othertopright, otherbottomleft, otherbottomright, position, other.position)) {
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