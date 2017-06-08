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

SDL_Window* displayWindow;

GLuint LoadTexture(const char *filepath) {
	int w, h, comp;
	unsigned char* image = stbi_load(filepath, &w, &h, &comp, STBI_rgb_alpha);

	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(image);
	return textureID;
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	#ifdef _WINDOWS
		glewInit();
	#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, 640, 360);

	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	// Set modelMatrix for each object

	GLuint cactusTexture = LoadTexture(RESOURCE_FOLDER"cactus.png");
	Matrix modelMatrixCactus;
	modelMatrixCactus.Translate(-2.0f, -1.5f, 0.0f);

	GLuint cloudTexture = LoadTexture(RESOURCE_FOLDER"cloud1.png");
	Matrix modelMatrixCloud;
	modelMatrixCloud.Translate(0.0f, 1.0f, 0.0f);
	float cloudSpeed = 0.01f;

	GLuint weightTexture = LoadTexture(RESOURCE_FOLDER"weight.png");
	Matrix modelMatrixWeight;
	modelMatrixWeight.Translate(2.0f, -1.5f, 0.0f);

	// Set projection matrix

	Matrix projectionMatrix;
	float windowWidth = 3.55f;
	float windowHeight = 2.0f;
	float windowDepth = 1.0f;
	projectionMatrix.setOrthoProjection(-windowWidth, windowWidth, -windowHeight, windowHeight, -windowDepth, windowDepth);

	// Set view matrix

	Matrix viewMatrix;

	glUseProgram(program.programID);

	float lastFrameTicks = 0.0f;

	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		// Do Game Logic

		modelMatrixCloud.Translate(sinf(ticks) * cloudSpeed, 0.0f, 0.0f);

		glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Start Drawing

		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);

		float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

		glEnableVertexAttribArray(program.texCoordAttribute);
		glEnableVertexAttribArray(program.positionAttribute);

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);

		// Draw Cactus

		program.setModelMatrix(modelMatrixCactus);
		glBindTexture(GL_TEXTURE_2D, cactusTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Draw Cloud

		program.setModelMatrix(modelMatrixCloud);
		glBindTexture(GL_TEXTURE_2D, cloudTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Draw Weight

		program.setModelMatrix(modelMatrixWeight);
		glBindTexture(GL_TEXTURE_2D, weightTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// End Drawing

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);


		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
