#pragma once
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
class Game
{
public:
	Game();
	bool Initialize();
	void GameLoop();
	void ShutDown();

	SDL_Texture* GetTexture(const std::string& fileName);

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

private:

	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();



	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;

	std::vector<class Actor*> gActor;
	std::vector<class Actor*> gPendingActors;

	//Map for referencing texture
	std::unordered_map<std::string, SDL_Texture*>gTexture;

	std::vector<class SpriteComponent*> gSprites;

	// Game should continue to run
	bool gIsRunning;
	bool gUpdatingActors;
	Uint32 gTicksCount;



	// Game-specific
	class Ship* mShip; // Player's ship
};

