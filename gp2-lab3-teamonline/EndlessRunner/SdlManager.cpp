#include "SdlManager.h"
#include "SDL.h"
#include <iostream>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

void SdlManager::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		auto error = SDL_GetError();
		printf("Failed to Init SDL_Init, %s\n", error);
		throw std::runtime_error(error);
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		auto error = IMG_GetError();
		printf("SDL_image could not initialize! SDL_image Error: %s\n", error);
		throw std::runtime_error(error);
	}

	if (TTF_Init() == -1)
	{
		auto error = TTF_GetError();
		printf("Failed to init TTF: %s\n", error);
		throw std::runtime_error(error);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		auto error = Mix_GetError();
		printf("Mix_OpenAudio Error: %s\n", error);
		throw std::runtime_error(error);
	}

	int channels = Mix_AllocateChannels(16);
	printf("Channels allocated: %i\n", channels);
}

SdlManager::~SdlManager()
{
	printf("Destroy SdlManager\n");
	SDL_Quit();
}
