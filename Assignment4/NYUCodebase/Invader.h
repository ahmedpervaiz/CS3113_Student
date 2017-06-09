#pragma once
#include "Entity.h"

class Invader : public Entity {
public:
	Invader(unsigned int textureID, int type, int xspace, int yspace);
	~Invader();
	void Update(float elapsed);
	void Draw(ShaderProgram *program, float elapsed);

	float animationElapsed = 0.0f;
	float framesPerSecond = 2.0f;
	int currentIndex = 0;
	int numFrames = 2;

	float sizeMod = 0.19f;

	int xspace, yspace;
	float moveElapsed;
	int type;
	unsigned int sheetID;
	SpriteSheet *sprite1;
	SpriteSheet *sprite2;
};