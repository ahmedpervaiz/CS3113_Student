#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#include <iostream>
#include "ShaderProgram.h"
#include "Matrix.h"

class Entity {
public:
	void Draw(ShaderProgram * program, Matrix * viewMatrix, Matrix * projectionMatrix);
	
	float x, y, rotation;
	Matrix modelMatrix;

	int textureID;

	float width;
	float height;

	float speed;
	float x_velocity;
	float y_velocity;

	bool isCollidingWith(Entity *other);
};