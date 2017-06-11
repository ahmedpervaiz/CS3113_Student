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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
	for (size_t i = 0; i < enemies.size(); i++) {
		delete enemies.at(i);
	}
	enemies.clear();
	entities.clear();

	if (levelIdx == 0) {
		memcpy(levelData, level1Data, LEVEL_WIDTH * LEVEL_HEIGHT);

		player->SetStartPos(0.6f, -5.8f);
		entities.push_back(player);

		Invader* enemy1 = new Invader(snailTexture);
		enemy1->position.x = 3.9f;
		enemy1->position.y = -3.4f;
		entities.push_back(enemy1);
		enemies.push_back(enemy1);

		Invader* enemy2 = new Invader(snailTexture);
		enemy2->position.x = 5.8f;
		enemy2->position.y = -4.8f;
		entities.push_back(enemy2);
		enemies.push_back(enemy2);
	}
	else if (levelIdx == 1) {
		memcpy(levelData, level2Data, LEVEL_WIDTH * LEVEL_HEIGHT);

		player->SetStartPos(8.6f, -9.8f);
		entities.push_back(player);

		Invader* enemy1 = new Invader(snailTexture);
		enemy1->position.x = 3.9f;
		enemy1->position.y = -3.4f;
		entities.push_back(enemy1);
		enemies.push_back(enemy1);

		Invader* enemy2 = new Invader(snailTexture);
		enemy2->position.x = 2.3f;
		enemy2->position.y = -4.6f;
		entities.push_back(enemy2);
		enemies.push_back(enemy2);
	}
	else if (levelIdx == 2) {
		memcpy(levelData, level3Data, LEVEL_WIDTH * LEVEL_HEIGHT);

		player->SetStartPos(8.6f, -9.8f);
		entities.push_back(player);

		Invader* enemy1 = new Invader(snailTexture);
		enemy1->position.x = 3.9f;
		enemy1->position.y = -3.4f;
		entities.push_back(enemy1);
		enemies.push_back(enemy1);

		Invader* enemy2 = new Invader(snailTexture);
		enemy2->position.x = 7.3f;
		enemy2->position.y = -8.2f;
		entities.push_back(enemy2);
		enemies.push_back(enemy2);
	}
}

void Setup() {

	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Mountain Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	jumpSound = Mix_LoadWAV("jumpSound.wav");
	bumpSound = Mix_LoadWAV("bumpSound.wav");
	levelSound = Mix_LoadWAV("levelSound.wav");
	deathSound = Mix_LoadWAV("deathSound.wav");
	bgmusic = Mix_LoadMUS("bgmusic.mp3");

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
	tilesetTexture = LoadTexture(RESOURCE_FOLDER"tiles_spritesheet.png");
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

	player = new Player(invadersTexture);

	gridX = new int(); gridY = new int();
	worldX = new float(); worldY = new float();
	buildLevel();

	Mix_PlayMusic(bgmusic, -1);
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
	if (worldX < 0) {
		*gridX = -1;
	}
	*gridY = (int)(-worldY / TILE_SIZE);
}

void tileCoordinatesToWorld(float *worldX, float *worldY, int gridX, int gridY) {
	*worldX = gridX * TILE_SIZE;
	*worldY = -gridY * TILE_SIZE;
}

enum TileType {EMPTY, SEMISOLID, SOLID, DOOR};

bool checkPlatformType(unsigned char tile, TileType tiletype) {
	if (tiletype == EMPTY) {
		switch (tile) {
		case 0:
		case 51:
		case 65:
		case 74:
			return true;
		default:
			return false;
		}
	}
	if (tiletype == SEMISOLID) {
		switch (tile) {
		case 157:
		case 156:
		case 148:
		case 120:
		case 119:
		case 86:
		case 85:
		case 84:
		case 58:
		case 4:
			return true;
		default:
			return false;
		}
	}
	if (tiletype == SOLID) {
		switch (tile) {
		case 84:
			return true;
		default:
			return false;
		}
	}
	if (tiletype == DOOR) {
		switch (tile) {
		case 65:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void Update(float elapsed) {

	// Render Inputs
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (gameState == 0) {
		player->acceleration.x = 0;
		if (keys[SDL_SCANCODE_LEFT]) {
			player->acceleration.x -= player->playerSpeed;
		}
		if (keys[SDL_SCANCODE_RIGHT]) {
			player->acceleration.x += player->playerSpeed;
		}
		if (keys[SDL_SCANCODE_SPACE] && player->collidedBottom) {
			player->velocity.y = player->playerJump;
			Mix_PlayChannel(1, jumpSound, 0);
		}
		if (!keys[SDL_SCANCODE_SPACE] && player->velocity.y > 0) {
			player->velocity.y = lerp(player->velocity.y, 0, 0.4f);
		}
		if (keys[SDL_SCANCODE_UP]) {
			worldToTileCoordinates(player->position.x, player->position.y, gridX, gridY);
			if (*gridX >= 0 && *gridX < LEVEL_WIDTH && *gridY >= 0 && *gridY < LEVEL_HEIGHT && checkPlatformType(levelData[*gridY][*gridX], DOOR)) {
				levelIdx++;
				if (levelIdx == LEVEL_COUNT) {
					levelIdx = 0;
				}
				Mix_PlayChannel(3, levelSound, 0);
				buildLevel();
			}
		}

		for (Invader* enemy : enemies) {
			if (enemy->collidedLeft) {
				enemy->direction = 1;
			}
			if (enemy->collidedRight) {
				enemy->direction = -1;
			}

			worldToTileCoordinates(enemy->position.x - enemy->size.x / 2, enemy->position.y - enemy->size.y / 2 - TILE_SIZE / 2, gridX, gridY);
			if (*gridX >= 0 && *gridX < LEVEL_WIDTH && *gridY >= 0 && *gridY < LEVEL_HEIGHT && checkPlatformType(levelData[*gridY][*gridX], EMPTY)) {
				enemy->direction = 1;
				enemy->velocity.x = 0;
			}
			worldToTileCoordinates(enemy->position.x + enemy->size.x / 2, enemy->position.y - enemy->size.y / 2 - TILE_SIZE / 2, gridX, gridY);
			if (*gridX >= 0 && *gridX < LEVEL_WIDTH && *gridY >= 0 && *gridY < LEVEL_HEIGHT && checkPlatformType(levelData[*gridY][*gridX], EMPTY)) {
				enemy->direction = -1;
				enemy->velocity.x = 0;
			}
			if (enemy->collidedBottom) {
				enemy->acceleration.x = enemy->playerSpeed * enemy->direction;
			}
		}

		for	(Entity* entity : entities) {
			entity->Update(elapsed);
			entity->ApplyY(elapsed);
			//Check collision
			worldToTileCoordinates(entity->position.x, entity->position.y - entity->size.y / 2 - TILE_SIZE / 4, gridX, gridY); // check bottom
			if (*gridY >= LEVEL_HEIGHT) {
				entity->Die();
				Mix_PlayChannel(4, deathSound, 0);
			} else if (*gridX >= 0 && *gridX < LEVEL_WIDTH && *gridY >= 0 && *gridY < LEVEL_HEIGHT && checkPlatformType(levelData[*gridY][*gridX], SEMISOLID) && entity->velocity.y <= 0) {
				entity->collidedBottom = true;
				entity->position.y = -*gridY * TILE_SIZE + entity->size.y / 2 + 0.01f + TILE_SIZE / 4;
				entity->velocity.y = 0;
			}
			worldToTileCoordinates(entity->position.x, entity->position.y + entity->size.y / 2, gridX, gridY); // check top
			if (*gridX >= 0 && *gridX < LEVEL_WIDTH && *gridY >= 0 && *gridY < LEVEL_HEIGHT && checkPlatformType(levelData[*gridY][*gridX], SOLID)) {
				entity->collidedTop = true;
				entity->position.y = -(*gridY + 1) * TILE_SIZE - entity->size.y / 2 - 0.01f;
				entity->velocity.y = 0;
			}

			entity->ApplyX(elapsed);
			worldToTileCoordinates(entity->position.x - entity->size.x / 2 - TILE_SIZE / 4, entity->position.y, gridX, gridY); // check left
			if (*gridX >= 0 && *gridX < LEVEL_WIDTH && *gridY >= 0 && *gridY < LEVEL_HEIGHT && checkPlatformType(levelData[*gridY][*gridX], SOLID)) {
				entity->collidedLeft = true;
				entity->position.x = (*gridX + 1) * TILE_SIZE + entity->size.x / 2 + 0.01f + TILE_SIZE / 4;
				entity->velocity.x = 0;
			}
			worldToTileCoordinates(entity->position.x + entity->size.x / 2 + TILE_SIZE / 4, entity->position.y, gridX, gridY); // check right
			if (*gridX >= 0 && *gridX < LEVEL_WIDTH && *gridY >= 0 && *gridY < LEVEL_HEIGHT && checkPlatformType(levelData[*gridY][*gridX], SOLID)) {
				entity->collidedRight = true;
				entity->position.x = *gridX * TILE_SIZE - entity->size.x / 2 - 0.01f - TILE_SIZE / 4;
				entity->velocity.x = 0;
			}
		}

		for (Invader* enemy : enemies) {
			if (player->isCollidingWith(*enemy)) {
				player->velocity.y = (player->position.y - enemy->position.y) * 12;
				player->velocity.x = (player->position.x - enemy->position.x) * 8;
				Mix_PlayChannel(2, bumpSound, 0);
			}
		}
	}
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (gameState == 0) {
		viewMatrix->identity();
		viewMatrix->Translate(-player->position.x, -player->position.y, -player->position.z);

		program->setModelMatrix(Matrix());
		program->setProjectionMatrix(*projectionMatrix);
		program->setViewMatrix(*viewMatrix);

		std::vector<float> vertexData;
		std::vector<float> texCoordData;

		int blankTiles = 0;

		for (int y = 0; y < LEVEL_HEIGHT; y++) {
			for (int x = 0; x < LEVEL_WIDTH; x++) {
				if (levelData[y][x] == 0) {
					blankTiles++;
				}
				else {
					float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
					float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;

					float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
					float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;

					vertexData.insert(vertexData.end(), {
						TILE_SIZE * x, -TILE_SIZE * y,
						TILE_SIZE * x, -TILE_SIZE * (y + 1),
						TILE_SIZE * (x + 1), -TILE_SIZE * (y + 1),

						TILE_SIZE * x, -TILE_SIZE * y,
						TILE_SIZE * (x + 1), -TILE_SIZE * (y + 1),
						TILE_SIZE * (x + 1), -TILE_SIZE * y
					});

					texCoordData.insert(texCoordData.end(), {
						u, v,
						u, v + spriteHeight,
						u + spriteWidth, v + spriteHeight,

						u, v,
						u + spriteWidth, v + spriteHeight,
						u + spriteWidth, v
					});
				}
			}
		}

		glEnableVertexAttribArray(program->positionAttribute);
		glEnableVertexAttribArray(program->texCoordAttribute);
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, &vertexData[0]);
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, &texCoordData[0]);

		glBindTexture(GL_TEXTURE_2D, tilesetTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6 * (LEVEL_HEIGHT * LEVEL_WIDTH - blankTiles));

		glDisableVertexAttribArray(program->positionAttribute);
		glDisableVertexAttribArray(program->texCoordAttribute);

		for (Entity* entity : entities) {
			entity->Draw(program);
		}
	}

	SDL_GL_SwapWindow(displayWindow);
}

void Cleanup() {
	Mix_FreeChunk(jumpSound);
	Mix_FreeChunk(bumpSound);
	Mix_FreeMusic(bgmusic);

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