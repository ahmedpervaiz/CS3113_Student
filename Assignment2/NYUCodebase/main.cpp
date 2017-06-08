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

	GLuint blockTexture = LoadTexture(RESOURCE_FOLDER"whiteblock.png");

	// Set projection matrix
	projectionMatrix = new Matrix();
	projectionMatrix->setOrthoProjection(-windowWidth, windowWidth, -windowHeight, windowHeight, -windowDepth, windowDepth);

	// Set view matrix
	viewMatrix = new Matrix();

	glUseProgram(program->programID);

	lastFrameTicks = 0.0f;
	done = false;

	leftPong = new Entity();
	leftPong->x = -paddleDistance;
	leftPong->width = ballWidth;
	leftPong->height = paddleHeight;
	leftPong->textureID = blockTexture;
	rightPong = new Entity();
	rightPong->x =  paddleDistance;
	rightPong->width = ballWidth;
	rightPong->height = paddleHeight;
	rightPong->textureID = blockTexture;
	pingBall = new Entity();
	pingBall->width = ballWidth;
	pingBall->height = ballWidth;
	pingBall->textureID = blockTexture;
	pingBall->x_velocity = ballSpeed;
	pingBall->y_velocity = ballSpeed;
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

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	// Render Inputs
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_W]) {
		// go up, left!
		leftPong->y_velocity =  paddleYspeed;
	}
	else if (keys[SDL_SCANCODE_S]) {
		// go down, left!
		leftPong->y_velocity = -paddleYspeed;
	}
	else {
		leftPong->y_velocity = 0;
	}
	leftPong->y += leftPong->y_velocity * elapsed;
	if (leftPong->y + leftPong->height / 2 > windowHeight) {
		leftPong->y = windowHeight - leftPong->height / 2;
		leftPong->y_velocity = 0;
	}
	else if (leftPong->y - leftPong->height / 2 < -windowHeight) {
		leftPong->y = -windowHeight + leftPong->height / 2;
		leftPong->y_velocity = 0;
	}

	if (keys[SDL_SCANCODE_UP]) {
		// go up, right!
		rightPong->y_velocity =  paddleYspeed;
	}
	else if (keys[SDL_SCANCODE_DOWN]) {
		// go down, right!
		rightPong->y_velocity = -paddleYspeed;
	}
	else {
		rightPong->y_velocity = 0;
	}
	rightPong->y += rightPong->y_velocity * elapsed;
	if (rightPong->y + rightPong->height / 2 > windowHeight) {
		rightPong->y = windowHeight - rightPong->height / 2;
		rightPong->y_velocity = 0;
	}
	else if (rightPong->y - rightPong->height / 2 < -windowHeight) {
		rightPong->y = -windowHeight + rightPong->height / 2;
		rightPong->y_velocity = 0;
	}

	pingBall->x += pingBall->x_velocity * elapsed;
	pingBall->y += pingBall->y_velocity * elapsed;
	if (pingBall->y + pingBall->height / 2 > windowHeight) {
		if (pingBall->y_velocity > 0) {
			pingBall->y_velocity = -pingBall->y_velocity;
		}
	}
	else if (pingBall->y - pingBall->height / 2 < -windowHeight) {
		if (pingBall->y_velocity < 0) {
			pingBall->y_velocity = -pingBall->y_velocity;
		}
	}

	if (pingBall->isCollidingWith(leftPong)) {
		if (pingBall->x  > leftPong->x && pingBall->x_velocity < 0) {
			pingBall->x_velocity = -pingBall->x_velocity;
		}
	}

	if (pingBall->isCollidingWith(rightPong)) {
		if (pingBall->x < rightPong->x && pingBall->x_velocity > 0) {
			pingBall->x_velocity = -pingBall->x_velocity;
		}
	}

	if (pingBall->x < -windowWidth) {
		pingBall->x_velocity = -pingBall->x_velocity;
		pingBall->x = 0;
		pingBall->y = 0;
		rightWins = 0.6f;
	}
	if (pingBall->x >  windowWidth) {
		pingBall->x_velocity = -pingBall->x_velocity;
		pingBall->x = 0;
		pingBall->y = 0;
		leftWins = 0.6f;
	}

	leftWins -= elapsed;
	rightWins -= elapsed;

	if (leftWins > 0) {
		leftPong->x = -paddleDistance - sinf(leftWins * 30) * 0.1f;
	}
	if (rightWins > 0) {
		rightPong->x = paddleDistance + sinf(rightWins * 30) * 0.1f;
	}
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	leftPong->Draw(program, viewMatrix, projectionMatrix);
	rightPong->Draw(program, viewMatrix, projectionMatrix);
	pingBall->Draw(program, viewMatrix, projectionMatrix);

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
