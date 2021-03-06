#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#include <iostream>
#include <vector>
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Entity.h"
#include "Invader.h"
#include "Player.h"
#include "PBullet.h"
#include <SDL_mixer.h>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

SDL_Window* displayWindow;
ShaderProgram* program;
SDL_Event event;
bool done;

Matrix *viewMatrix;
Matrix *projectionMatrix;
float windowWidth = 3.55f;
float windowHeight = 2.0f;
float windowDepth = 1.0f;

#define LEVEL_COUNT 3
#define LEVEL_HEIGHT 26
#define LEVEL_WIDTH 26
#define TILE_SIZE 0.4f
#define SPRITE_COUNT_X 14
#define SPRITE_COUNT_Y 14

unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH];
unsigned char level1Data[LEVEL_HEIGHT][LEVEL_WIDTH] = {
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  51,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  74,  65,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 148, 119, 120,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  84,  84,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  84,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0, 148, 119, 119, 120,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0, 148, 119, 120,   0,   0,   0,  84,  84, 119, 119, 119, 119, 119, 119,  84,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0, 176,   0,   0,   0,   0,  84,  84, 118, 118, 118, 118, 118, 118,  84,   0,   0,   0,   0,   0,   0,   0 },
	{  46,   0,   0,   0,   0, 176,   0,   0,   0,   0,  84,  84,  49,  49,  49,  49,  49,  49,  84,   0,   0,   0,   0,   0,   0,   0 },
	{ 148, 119, 119, 119, 119, 119, 119, 119,  84,   0, 148, 119, 119, 119, 119, 119, 119, 119, 120,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0, 148, 119,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }
};
unsigned char level2Data[LEVEL_HEIGHT][LEVEL_WIDTH] = {
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  51,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  74,  65,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  86,  85,  58,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  84,  84,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  84,  84,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,  86,  85,  58,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,  86,  85,  85,  85,  58,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,  86,  85,  58,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,  86,  85,  85,  58,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  58,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  86,  85,  84,   0,   0,   0,  60,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,  86,  85,  58,   0,   0,   0,   0 }
};
unsigned char level3Data[LEVEL_HEIGHT][LEVEL_WIDTH] = {
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  51,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  74,  65,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4, 156, 157,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  84,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  84,  84,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  84,  84,  84,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4, 156, 156, 157,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   4, 156, 156, 157,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   4, 156, 156, 157,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   4,  84,  84, 156,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84,  84,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  84, 156, 156,  84,  84, 156,  84,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  60,   0,   0,   0,   0,   0 },
	{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4, 156, 156, 157,   0,   0,   0,   0 }
};

float ticks;
float elapsed;
float lastFrameTicks;

int *gridX, *gridY;
float *worldX, *worldY;

int levelIdx;
int playScore;
int gameState;
GLuint blockTexture, invadersTexture, snailTexture, fontTexture, tilesetTexture;

Mix_Chunk *jumpSound, *bumpSound, *levelSound, *deathSound;
Mix_Music *bgmusic;

Player* player;
std::vector<Invader*> enemies;
std::vector<Entity*> entities;