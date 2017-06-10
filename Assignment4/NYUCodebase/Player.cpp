#include "Player.h"

Player::Player(unsigned int spriteID)
{
	entityType = EntityType::ENTITY_PLAYER;
	sprite = new SpriteSheet(spriteID, 369, 5, 65, 91, 512, 512, sizeMod);

	size.x = 65 * sizeMod / 640 * 7;
	size.y = 91 * sizeMod / 360 * 4;
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
