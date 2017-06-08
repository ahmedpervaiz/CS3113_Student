#include "Entity.h"

void Entity::Draw(ShaderProgram *program, Matrix *viewMatrix, Matrix *projectionMatrix) {
	modelMatrix.setPosition(x, y, 0);
	modelMatrix.setScale(width, height, 1);

	program->setProjectionMatrix(*projectionMatrix);
	program->setModelMatrix(modelMatrix);
	program->setViewMatrix(*viewMatrix);

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

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

bool Entity::isCollidingWith(Entity *other) {
	if (y - height / 2 > other->y + other->height / 2 ||
		y + height / 2 < other->y - other->height / 2 ||
		x - width / 2 > other->x + other->width / 2 ||
		x + width / 2 < other->x - other->width / 2) {
		return false;
	}
	return true;
}