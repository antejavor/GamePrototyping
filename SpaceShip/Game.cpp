#include "Game.h"
#include "SDL2/SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Component.h"
#include "Ship.h"
#include "BGSpriteComponent.h"

Game::Game() :gWindow{ nullptr }, gRenderer{ nullptr }, gIsRunning{ true }, gUpdatingActors{ false }{}


bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0){
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	gWindow = SDL_CreateWindow(
		"SpaceShip", // Window title
		100,   // Top left x-coordinate of window
		100,   // Top left y-coordinate of window
		1024,  // Width of window
		768,   // Height of window
		0      // Flags 
	);
	if (!gWindow){
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	gRenderer = SDL_CreateRenderer(
		gWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!gRenderer){
		SDL_Log("Faild to create renderer: %s", SDL_GetError());
		return false;
	}
	
	int imgResult = IMG_Init(IMG_INIT_PNG);
	if (!imgResult) {
		SDL_Log("Faild to init image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	gIsRunning = true;
	return true;
}


void Game::GameLoop() {

	while (gIsRunning) {
		
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}

}

void Game::ShutDown() {

	while (!gActor.empty()) {
		delete gActor.back();
	}

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();


}

void Game::UpdateGame() {

	// Compute delta time
// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), gTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - gTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	gTicksCount = SDL_GetTicks();

	// Update all actors
	gUpdatingActors = true;
	for (auto actor : gActor)
	{
		actor->Update(deltaTime);
	}
	gUpdatingActors = false;

	// Move any pending actors to mActors
	for (auto pending : gPendingActors)
	{
		gActor.emplace_back(pending);
	}
	gPendingActors.clear();

	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : gActor)
	{
		if (actor->GetState() == Actor::State::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// Delete dead actors (which removes them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}

}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		gIsRunning = false;
	}

	// Process ship input
	mShip->ProcessKeyboard(state);
}


void Game::AddActor(Actor* actor) {
	if (gUpdatingActors) {
		gPendingActors.emplace_back(actor);
	}
	else {
		gActor.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(gPendingActors.begin(), gPendingActors.end(), actor);
	if (iter != gPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, gPendingActors.end() - 1);
		gPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(gActor.begin(), gActor.end(), actor);
	if (iter != gActor.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, gActor.end() - 1);
		gActor.pop_back();
	}

}


SDL_Texture* Game::GetTexture(const std::string& fileName) {
	
	SDL_Texture* tex = nullptr;
	// Is the texture already in the map?
	auto iter = gTexture.find(fileName);
	if (iter != gTexture.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(gRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		gTexture.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = gSprites.begin();
	for (;
		iter != gSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	gSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// (We can't swap because it ruins ordering)
	auto iter = std::find(gSprites.begin(), gSprites.end(), sprite);
	gSprites.erase(iter);
}


void Game::LoadData()
{
	// Create player's ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);

	// Create actor for the background (this doesn't need a subclass)
	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));
	// Create the "far back" background
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);
	// Create the closer background
	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!gActor.empty())
	{
		delete gActor.back();
	}

	// Destroy textures
	for (auto i : gTexture)
	{
		SDL_DestroyTexture(i.second);
	}
	gTexture.clear();
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);

	// Draw all sprite components
	for (auto sprite : gSprites)
	{
		sprite->Draw(gRenderer);
	}

	SDL_RenderPresent(gRenderer);
}