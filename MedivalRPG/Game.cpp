#include "Game.h"

Game::Game() : mWindow{ nullptr }, mIsRunning { true }
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
	mIsRunning = true;
	return true;
}
void Game::Shutdown()
{
	SDL_DestroyWindow(mWindow);
	SDL_Quit();

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
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

}
void Game::UpdateGame()
{
}

void Game::GenerateOutput()
{
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
