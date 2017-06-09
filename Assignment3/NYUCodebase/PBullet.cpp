#include "PBullet.h"

PBullet::PBullet(unsigned int spriteID)
{
	sprite = new SpriteSheet(spriteID, 487, 394, 28, 51, 585, 718, sizeMod);

	size.x = 28 * sizeMod / 640 * 7;
	size.y = 51 * sizeMod / 360 * 4;
}

PBullet::~PBullet()
{

}

void PBullet::Update(float elapsed)
{
	Entity::Update(elapsed);
}
