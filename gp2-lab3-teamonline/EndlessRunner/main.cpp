
#include <iostream>
#include "SDL.h"
#include "SdlManager.h"
#include "Game.h"

int main(int argc, char* args[])
{
	SdlManager sdlManager;
	sdlManager.Init();

	Game game;
	game.Run();
	
	return 0;
}
