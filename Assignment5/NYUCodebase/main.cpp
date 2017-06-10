#include "main.h"

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

void DrawText(ShaderProgram *program, int fontTexture, std::string text, float x, float y, float size, float spacing) {
	Matrix modelMatrix;
	modelMatrix.setPosition(x, y, 0);

	program->setModelMatrix(modelMatrix);
	program->setProjectionMatrix(*projectionMatrix);
	program->setViewMatrix(Matrix());

	float texture_size = 1.0f / 16.0f;

	std::vector<float> vertexData;
	std::vector<float> texCoordData;

	for (int i = 0; i < text.size(); i++) {
		int spriteIndex = (int)text[i];

		float texture_x = (float)(spriteIndex % 16) / 16.0f;
		float texture_y = (float)(spriteIndex / 16) / 16.0f;

		vertexData.insert(vertexData.end(), {
			((size * (1 + spacing)) * i) + (-0.5f * size), 0.5f * size,
			((size * (1 + spacing)) * i) + (-0.5f * size), -0.5f * size,
			((size * (1 + spacing)) * i) + (0.5f * size), 0.5f * size,
			((size * (1 + spacing)) * i) + (0.5f * size), -0.5f * size,
			((size * (1 + spacing)) * i) + (0.5f * size), 0.5f * size,
			((size * (1 + spacing)) * i) + (-0.5f * size), -0.5f * size
		});

		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size
		});
	}
	

	glEnableVertexAttribArray(program->texCoordAttribute);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());

	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6 * text.size());

	// End Drawing
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void buildLevel() {
	memcpy(levelData, level1Data, LEVEL_WIDTH * LEVEL_HEIGHT);
}

void Setup() {

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

	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	fontTexture = LoadTexture(RESOURCE_FOLDER"font1.png");
	blockTexture = LoadTexture(RESOURCE_FOLDER"whiteblock.png");
	invadersTexture = LoadTexture(RESOURCE_FOLDER"p1_spritesheet.png");
	snailTexture = LoadTexture(RESOURCE_FOLDER"enemies_spritesheet.png");

	// Set projection matrix
	projectionMatrix = new Matrix();
	projectionMatrix->setOrthoProjection(-windowWidth, windowWidth, -windowHeight, windowHeight, -windowDepth, windowDepth);

	// Set view matrix
	viewMatrix = new Matrix();

	glUseProgram(program->programID);

	lastFrameTicks = 0.0f;
	done = false;

	gameState = 0;

	float sizeMod = 0.3f;
	float bugAdjust = 0.8f;
	srand(time(nullptr));

	Entity* e1 = new Entity();
	e1->position.x = -1.5f;
	e1->position.y = -1.5f;
	e1->velocity.x = 0.003f;
	e1->velocity.y = 0.003f;
	e1->rotation = rand() / 1000.0f;
	e1->sprite = new SpriteSheet(blockTexture, 0, 0, 70, 70, 70, 70, sizeMod);
	e1->size.x = 70 * sizeMod / 640 * 7 * bugAdjust;
	e1->size.y = 70 * sizeMod / 360 * 4 * bugAdjust;
	entities.push_back(e1);

	sizeMod = 0.4f;

	Entity* e2 = new Entity();
	e2->position.x = 1.5f;
	e2->position.y = -1.5f;
	e2->velocity.x = -0.004f;
	e2->velocity.y = 0.003f;
	e2->rotation = rand() / 1000.0f;
	e2->sprite = new SpriteSheet(blockTexture, 0, 0, 70, 70, 70, 70, sizeMod);
	e2->size.x = 70 * sizeMod / 640 * 7 * bugAdjust;
	e2->size.y = 70 * sizeMod / 360 * 4 * bugAdjust;
	entities.push_back(e2);

	sizeMod = 0.6f;

	Entity* e3 = new Entity();
	e3->position.x = 0;
	e3->position.y = 1.5f;
	e3->velocity.x = 0.0f;
	e3->velocity.y = -0.003f;
	e3->rotation = rand() / 1000.0f;
	e3->sprite = new SpriteSheet(blockTexture, 0, 0, 70, 70, 70, 70, sizeMod);
	e3->size.x = 70 * sizeMod / 640 * 7 * bugAdjust;
	e3->size.y = 70 * sizeMod / 360 * 4 * bugAdjust;
	entities.push_back(e3);

	gridX = new int(); gridY = new int();
	worldX = new float(); worldY = new float();
	buildLevel();
}

void ProcessEvents() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
}

void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
	*gridX = (int)(worldX / TILE_SIZE);
	*gridY = (int)(-worldY / TILE_SIZE);
}

void tileCoordinatesToWorld(float *worldX, float *worldY, int gridX, int gridY) {
	*worldX = gridX * TILE_SIZE;
	*worldY = -gridY * TILE_SIZE;
}

void Update(float elapsed) {

	// Render Inputs
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (gameState == 0) {
		for (int i = 0; i < entities.size(); i++) {
			entities.at(i)->Update(elapsed);
			entities.at(i)->ApplyX(elapsed);
			entities.at(i)->ApplyY(elapsed);
		}

		for (int i = 0; i < entities.size(); i++) {
			for (int j = i + 1; j < entities.size(); j++) {
				entities.at(i)->CollisionFix(*entities.at(j));
			}
		}
	}
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (gameState == 0) {
		program->setModelMatrix(Matrix());
		program->setProjectionMatrix(*projectionMatrix);
		program->setViewMatrix(*viewMatrix);

		for (Entity* entity : entities) {
			entity->Draw(program);
		}
	}

	SDL_GL_SwapWindow(displayWindow);
}

void Cleanup() {
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	Setup();

	while (!done) {
		ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		float fixedElapsed = elapsed;
		if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
			fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
		}
		while (fixedElapsed >= FIXED_TIMESTEP) {
			fixedElapsed -= FIXED_TIMESTEP;
			Update(FIXED_TIMESTEP);
		}

		ProcessEvents();
		Update(FIXED_TIMESTEP);
		Render();
	}

	Cleanup();

	return 0;
}
