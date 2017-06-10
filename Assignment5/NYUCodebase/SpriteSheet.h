#pragma once
#include "ShaderProgram.h"

class SpriteSheet {
public:
	SpriteSheet();
	SpriteSheet(unsigned int textureID, int u, int v, int width, int height, int imageWidth, int imageHeight, float size);
	
	void Draw(ShaderProgram *program);

	unsigned int textureID;
	float u, v, width, height, size;
};