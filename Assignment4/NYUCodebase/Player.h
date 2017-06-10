#pragma once
#include "Entity.h"

class Player : public Entity {
public:
	Player(unsigned int spriteID);
	~Player();
	void Update(float elapsed);
	void Draw(ShaderProgram *program);

	float sizeMod = 0.22f;
	float playerSpeed = 0.33f;
	float playerJump = 2.3f;
};