#include "Invader.h"

Invader::Invader(unsigned int spriteID)
{
	entityType = EntityType::ENTITY_ENEMY;
	sprite = new SpriteSheet(spriteID, 67, 87, 57, 31, 353, 153, sizeMod);

	size.x = 57 * sizeMod / 640 * 7;
	size.y = 31 * sizeMod / 360 * 4;
}

Invader::~Invader()
{
	delete sprite;
}

void Invader::Update(float elapsed)
{
	Entity::Update(elapsed);
}

void Invader::Draw(ShaderProgram * program)
{
	Entity::Draw(program);
}
