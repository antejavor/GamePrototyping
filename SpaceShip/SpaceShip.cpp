#include "Game.h"
int main(int argc, char* argv[])
{
	Game game;
	bool status = game.Initialize();

	if (status) {
		game.GameLoop();
	}

	game.ShutDown();
	return 0;

}