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
#include <vector>
#include "ShaderProgram.h"
#include "SpriteSheet.h"
#include "Matrix.h"
#include "Vector3.h"
#include "Point.h"

enum EntityType {ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_COIN};

class Entity {
public:
	Entity();
	~Entity();

	void Update(float elapsed);
	void ApplyY(float elapsed);
	void ApplyX(float elapsed);

	void Draw(ShaderProgram * program);
	void CollisionFix(Entity& other);
	bool isCollidingWith(const Entity& other);
	void checkBox(float u, float v, float width, float height);

	SpriteSheet *sprite;
	
	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 size;

	float friction_x = 0.0f;
	float friction_y = 0.0f;
	float gravity_x = 0.0f;
	float gravity_y = 0.0f;

	float rotation;

	bool isStatic;
	EntityType entityType;

	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
};