#include "Game.h"
#include <math.h>

Game::Game() : mWindow{ nullptr }, mRenderer{ nullptr }, mIsRunning{ true },
mPaddlePos{ 10, 384 }, mBallPos{ 512, 384 }, mBallVel{ -200.0f, 235.0f}, mTicksCount{ 0 }
{

}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	mWindow = SDL_CreateWindow(
		"MedivalRPG", // Window title
		100,   // Top left x-coordinate of window
		100,   // Top left y-coordinate of window
		1024,  // Width of window
		768,   // Height of window
		0      // Flags 
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!mRenderer)
	{
		SDL_Log("Faild to create renderer: %s", SDL_GetError());
		return false;
	}


	mIsRunning = true;
	return true;
}

void Game::ProcessInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]){
		mIsRunning = false;
	}
	
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}

}
void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	mTicksCount = SDL_GetTicks();

	//For debugging purpose
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	if (mPaddleDir != 0) {
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
	}

	if (mPaddlePos.y < 0){
		mPaddlePos.y = 0;

	}
	else if (mPaddlePos.y > (768.0f - 30)){

		mPaddlePos.y = 768.0f - 30;

	}


	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	//Check if position is on top of the screen and check if ball changed 
	//direction in previous frame to avoid double change
	if (mBallPos.y <= 15 && mBallVel.y < 0.0f)
	{

		mBallVel.y *= -1;

	}
	else if (mBallPos.y >= 768 - 15 && mBallVel.y > 0.0f) {
		mBallVel.y *= -1;
	} 
	
	if (mBallPos.x >= 1024 - 15 && mBallVel.x > 0.0f) {
		mBallVel.x *= -1;
	}
	
	int diff = static_cast<int>(mPaddlePos.y - mBallPos.y);

	if (diff > -30 && diff < 30 && mBallPos.x <= 17 && mBallPos.x >= 7 && mBallVel.x < 0.0f) {
		mBallVel.x *= -1.0f;
	}
}

void Game::GenerateOutput()
{

	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		255,
		255
	);

	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	SDL_Rect wall{
		static_cast<int>(mPaddlePos.x - 15 / 2),
		static_cast<int>(mPaddlePos.y - 15 / 2),
		15,
		60
	};

	SDL_Rect ball{
		static_cast<int>(mBallPos.x - 15 / 2),
		static_cast<int>(mBallPos.y - 15 / 2),
		15,
		15
	};

	SDL_RenderFillRect(mRenderer, &wall);
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_RenderPresent(mRenderer);

}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();

}

void Game::RunLoop()
{
	while (mIsRunning)

	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();

	}

}
