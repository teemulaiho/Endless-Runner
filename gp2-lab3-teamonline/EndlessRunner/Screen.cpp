#include "Screen.h"
#include "Game.h"
#include <iostream>

void Screen::Init()
{
	window = SDL_CreateWindow("Endless Runner",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		W,
		H,
		SDL_WINDOW_ALLOW_HIGHDPI);
}

Screen::Screen(Game* game)
{
	this->game = game;
	window = NULL;
}

Screen::~Screen()
{
	printf("Destroy Screen\n");
	SDL_DestroyWindow(window);
}
