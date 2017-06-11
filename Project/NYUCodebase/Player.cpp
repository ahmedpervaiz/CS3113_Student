#include "Player.h"

Player::Player(unsigned int spriteID)
{
	entityType = EntityType::ENTITY_PLAYER;
	sprite = new SpriteSheet(spriteID, 369, 5, 65, 91, 512, 512, sizeMod);

	size.x = 65 * sizeMod / 640 * 3.55f;
	size.y = 91 * sizeMod / 360 * 2.00f;
}

Player::~Player()
{
	delete sprite;
}

void Player::Update(float elapsed)
{
	Entity::Update(elapsed);
}

void Player::Draw(ShaderProgram * program)
{
	Entity::Draw(program);
}

void Player::SetStartPos(float x, float y)
{
	startPos.x = x;
	startPos.y = y;
	position.x = startPos.x;
	position.y = startPos.y;
}

void Player::ResetPos() {
	position.x = startPos.x;
	position.y = startPos.y;
}

void Player::Die() {
	ResetPos();
}