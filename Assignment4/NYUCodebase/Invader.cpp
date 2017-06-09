#include "Invader.h"

Invader::Invader(unsigned int textureID, int type, int xspace, int yspace)
{
	this->type = type;
	sheetID = textureID;

	this->xspace = xspace;
	this->yspace = yspace;

	switch (type) {
	case 0:
		sprite1 = new SpriteSheet(sheetID, 312, 14, 80, 80, 585, 718, sizeMod);
		sprite2 = new SpriteSheet(sheetID, 428, 14, 80, 80, 585, 718, sizeMod);
		size.x = 80 * sizeMod;
		size.y = 80 * sizeMod;
		break;
	case 1:
		sprite1 = new SpriteSheet(sheetID, 300, 134, 100, 80, 585, 718, sizeMod);
		sprite2 = new SpriteSheet(sheetID, 421, 134, 100, 80, 585, 718, sizeMod);
		size.x = 100 * sizeMod;
		size.y = 80 * sizeMod;
		break;
	case 2:
		sprite1 = new SpriteSheet(sheetID, 19, 14, 110, 80, 585, 718, sizeMod);
		sprite2 = new SpriteSheet(sheetID, 165, 14, 110, 80, 585, 718, sizeMod);
		size.x = 110 * sizeMod;
		size.y = 80 * sizeMod;
		break;
	default:
		sprite1 = new SpriteSheet(sheetID, 19, 134, 120, 80, 585, 718, sizeMod);
		sprite2 = new SpriteSheet(sheetID, 165, 134, 120, 80, 585, 718, sizeMod);
		size.x = 120 * sizeMod;
		size.y = 80 * sizeMod;
		break;
	}
	size.x = size.x / 640 * 7;
	size.y = size.y / 360 * 4;

	sprite = sprite1;
}

Invader::~Invader()
{
	delete sprite1;
	delete sprite2;
}

void Invader::Update(float elapsed) {
	position.x = xspace * 0.23f;
	position.y = yspace  * 0.26f;
}

void Invader::Draw(ShaderProgram * program, float elapsed)
{
	animationElapsed += elapsed;

	if (animationElapsed > 1.0f / framesPerSecond) {
		currentIndex++;
		animationElapsed = 0.0f;

		if (currentIndex > numFrames - 1) {
			currentIndex = 0;
		}
	}

	switch (currentIndex) {
	case 0:
		sprite = sprite1;
		break;
	case 1:
		sprite = sprite2;
		break;
	default:
		break;
	}

	Entity::Draw(program);
}
