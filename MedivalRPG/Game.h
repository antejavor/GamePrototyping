#pragma once
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "Vector2.h"

class Game
{

public:

	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:

	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;


	// Game should continue to run
	bool mIsRunning;


	Vector2 mPaddlePos;
	Vector2 mBallPos;
	Vector2 mBallVel;
	int mPaddleDir = 0;
	
	Uint32 mTicksCount;
};

