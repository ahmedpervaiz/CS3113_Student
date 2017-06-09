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

float ticks;
float elapsed;
float lastFrameTicks;

int playHiScore;
int playScore;
int gameState;
GLuint blockTexture, invadersTexture, fontTexture;

float invadeTicks;
int invadeDirection = 1;
std::vector<Invader*> invaders;
Player* player;
std::vector<PBullet*> pbullets;

bool spacekeyState;