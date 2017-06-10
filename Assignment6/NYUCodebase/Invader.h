#pragma once
#include "Entity.h"

class Invader : public Entity {
public:
	Invader(unsigned int spriteID);
	~Invader();
	void Update(float elapsed);
	void Draw(ShaderProgram *program);

	float sizeMod = 0.44f;
	float playerSpeed = 0.25f;
	float playerJump = 2.3f;

	int direction = 1;
};