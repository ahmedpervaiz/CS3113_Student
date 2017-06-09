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
#include "SpriteSheet.h"
#include "Matrix.h"
#include "Vector3.h"

class Entity {
public:
	Entity();
	~Entity();

	void Update(float elapsed);

	void Draw(ShaderProgram * program);
	
	Vector3 position;
	Vector3 velocity;
	Vector3 size;

	float rotation;

	Matrix modelMatrix;
	Matrix *viewMatrix, *projectionMatrix;

	SpriteSheet *sprite;

	bool isCollidingWith(Entity *other);
};