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
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Entity.h"

SDL_Window* displayWindow;
ShaderProgram* program;
SDL_Event event;
bool done;

Matrix *viewMatrix;
Matrix *projectionMatrix;
float windowWidth = 3.55f;
float windowHeight = 2.0f;
float windowDepth = 1.0f;
float lastFrameTicks;

float paddleYspeed = 2.6f;
float ballSpeed = 3.2f;

float ballWidth = 0.25f;
float paddleHeight = 0.9f;
float paddleDistance = 2.8f;

float leftWins, rightWins;

Entity *leftPong;
Entity *rightPong;
Entity *pingBall;