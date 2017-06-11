#pragma once
#include "Entity.h"

class Player : public Entity {
public:
	Player(unsigned int spriteID);
	~Player();
	void Update(float elapsed);
	void Draw(ShaderProgram *program);
	void SetStartPos(float x, float y);
	void ResetPos();

	void Die();

	Vector3 startPos;
	float sizeMod = 0.32f;
	float playerSpeed = 0.37f;
	float playerJump = 2.3f;
};