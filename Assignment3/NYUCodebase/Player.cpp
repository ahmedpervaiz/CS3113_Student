#include "Player.h"

Player::Player(unsigned int spriteID)
{
	sprite = new SpriteSheet(spriteID, 150, 638, 73, 52, 585, 718, sizeMod);

	size.x = 73 * sizeMod / 640 * 7;
	size.y = 52 * sizeMod / 360 * 4;
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
