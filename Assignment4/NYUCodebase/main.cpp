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

	blockTexture = LoadTexture(RESOURCE_FOLDER"whiteblock.png");
	invadersTexture = LoadTexture(RESOURCE_FOLDER"invaders.gif");
	fontTexture = LoadTexture(RESOURCE_FOLDER"font1.png");

	// Set projection matrix
	projectionMatrix = new Matrix();
	projectionMatrix->setOrthoProjection(-windowWidth, windowWidth, -windowHeight, windowHeight, -windowDepth, windowDepth);

	// Set view matrix
	viewMatrix = new Matrix();

	glUseProgram(program->programID);

	lastFrameTicks = 0.0f;
	done = false;

	gameState = 0;
}

void ProcessEvents() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
}

float lerp(float v1, float v2, float t) {
	return v1 + t*(v2-v1);
}

void generateScene() {
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 4; j++) {
			Invader* invader = new Invader(invadersTexture, j, -3 + i * 2, 2 + j);
			invaders.push_back(invader);
		}
	}
	player = new Player(invadersTexture);
	player->position.y = -1.7f;
}

void Update() {
	ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	// Render Inputs
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (gameState == 0 && keys[SDL_SCANCODE_SPACE]) {
		gameState = 1;

		generateScene();
	}
	else if (gameState == 1) {
		player->velocity.x = 0;
		if (keys[SDL_SCANCODE_LEFT]) {
			player->velocity.x = -player->playerSpeed;
		}
		if (keys[SDL_SCANCODE_RIGHT]) {
			player->velocity.x = player->playerSpeed;
		}
		if (keys[SDL_SCANCODE_SPACE] && !spacekeyState && pbullets.size() < 10) {
			PBullet* pbullet = new PBullet(invadersTexture);
			pbullet->position = player->position;
			pbullet->velocity.y = 1.8f;
			pbullets.push_back(pbullet);
		}
		player->Update(elapsed);
		if (player->position.x > 3.55f) {
			player->position.x = 3.55f;
		}
		if (player->position.x < -3.55f) {
			player->position.x = -3.55f;
		}

		invadeTicks += elapsed;

		int minx = 20, maxx = -20;
		for (int i = 0; i < invaders.size(); i++) {
			invaders.at(i)->Update(elapsed);
			if (invaders.at(i)->xspace < minx) {
				minx = invaders.at(i)->xspace;
			}
			if (invaders.at(i)->xspace > maxx) {
				maxx = invaders.at(i)->xspace;
			}
		}

		if (invadeTicks > 1) {
			if (invadeDirection == 0) { //GO LEFT 
				if (minx > -12) {
					for (int i = 0; i < invaders.size(); i++) {
						invaders.at(i)->xspace--;
					}
				}
				else {
					for (int i = 0; i < invaders.size(); i++) {
						invaders.at(i)->yspace--;
					}
					invadeDirection = 1; //GO RIGHT
				}
			}
			else if (invadeDirection == 1) { //GO DOWN
				if (maxx < 12) {
					for (int i = 0; i < invaders.size(); i++) {
						invaders.at(i)->xspace++;
					}
				}
				else {
					for (int i = 0; i < invaders.size(); i++) {
						invaders.at(i)->yspace--;
					}
					invadeDirection = 0;
				}
			}
			invadeTicks = 0;
		}

		for (int i = 0; i < pbullets.size(); i++) {
			pbullets.at(i)->Update(elapsed);

			if (pbullets.at(i)->position.y > 2.0f) {
				delete pbullets.at(i);
				pbullets.erase(pbullets.begin() + i);
				i--;
			}
			else {
				for (int j = 0; j < invaders.size(); j++) {
					if (pbullets.at(i)->isCollidingWith(invaders.at(j))) {
						delete pbullets.at(i);
						pbullets.erase(pbullets.begin() + i);
						i--;
						delete invaders.at(j);
						invaders.erase(invaders.begin() + j);
						j--;
						playScore += 100;
						break;
					}
				}

				if (invaders.size() == 0) {
					gameState = 0;
					if (playScore > playHiScore)
						playHiScore = playScore;
				}
			}
		}

		for (int i = 0; i < invaders.size(); i++) {
			if (player->isCollidingWith(invaders.at(i))) {
				gameState = 0;
				if (playScore > playHiScore)
					playHiScore = playScore;
			}
		}
	}

	spacekeyState = keys[SDL_SCANCODE_SPACE];
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (gameState == 0) {
		DrawText(program, fontTexture, "Invaders", -1.3f, 0.4f, 1, -0.6f);
		DrawText(program, fontTexture, "Press SPACE to Start", -1.5f, -0.6f, 0.4f, -0.6f);

		DrawText(program, fontTexture, "High Score: " + std::to_string(playHiScore), -3.4f, -1.7f, 0.4f, -0.6f);
	}
	else if (gameState == 1) {
		program->setProjectionMatrix(*projectionMatrix);
		program->setViewMatrix(*viewMatrix);

		for (int i = 0; i < invaders.size(); i++) {
			invaders.at(i)->Draw(program, elapsed);
		}
		player->Draw(program);
		for (int i = 0; i < pbullets.size(); i++) {
			pbullets.at(i)->Draw(program);
		}
		DrawText(program, fontTexture, "Score: " + std::to_string(playScore), -3.4f, -1.7f, 0.4f, -0.6f);
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
		ProcessEvents();
		Update();
		Render();
	}

	Cleanup();

	return 0;
}
