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

enum EntityType {ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_COIN};

class Entity {
public:
	Entity();
	~Entity();

	void Update(float elapsed);
	void Draw(ShaderProgram * program);
	bool isCollidingWith(Entity *other);

	SpriteSheet *sprite;
	
	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 size;

	float rotation;

	bool isStatic;
	EntityType entityType;
};