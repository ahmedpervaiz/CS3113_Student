#include "SpriteSheet.h"

SpriteSheet::SpriteSheet()
{
	textureID = 0;
	width = 1;
	height = 1;
	size = 1;
}

SpriteSheet::SpriteSheet(unsigned int textureID, int u, int v, int width, int height, int imageWidth, int imageHeight, float size)
{
	this->textureID = textureID;
	this->u = (float)u / imageWidth;
	this->v = (float)v / imageHeight;
	this->width = (float)width / imageWidth;
	this->height = (float)height / imageHeight;
	this->size = size;
}

void SpriteSheet::Draw(ShaderProgram * program)
{
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLfloat texCoords[] = {
		u, v,
		u, v + height,
		u + width, v,
		u + width, v + height,
		u + width, v,
		u, v + height
	};

	float aspect = width / height;
	float vertices[] = {
		-0.5 * size * aspect, 0.5 * size,
		-0.5 * size * aspect, -0.5 * size,
		0.5 * size * aspect, 0.5 * size,
		0.5 * size * aspect, -0.5 * size,
		0.5 * size * aspect, 0.5 * size,
		-0.5 * size * aspect, -0.5 * size
	};

	glEnableVertexAttribArray(program->texCoordAttribute);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// End Drawing
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}
