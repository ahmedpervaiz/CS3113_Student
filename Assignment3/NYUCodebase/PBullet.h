#pragma once
#include "Entity.h"
#include "Invader.h"

class PBullet : public Entity {
public:
	PBullet(unsigned int spriteID);
	~PBullet();

	void Update(float elapsed);

	float sizeMod = 0.1f;
};