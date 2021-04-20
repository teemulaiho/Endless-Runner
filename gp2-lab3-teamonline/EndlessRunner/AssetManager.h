#pragma once

#include "SDL.h"
#include <map>
#include <string>
#include <SDL_mixer.h>
#include <iostream>
#include "SDL_ttf.h"

class Game;
using namespace std;

struct Music
{
	Mix_Music* data;
	Music(string filename)
	{
		data = Mix_LoadMUS(filename.c_str());
		if (data == NULL)
		{
			throw runtime_error(Mix_GetError());
		}
	}
	~Music()
	{
		Mix_FreeMusic(data);
		data = NULL;
	}
};

struct Sound
{
	Mix_Chunk* data;
	Sound(string filename)
	{
		data = Mix_LoadWAV(filename.c_str());
		if (data == NULL)
		{
			throw runtime_error(Mix_GetError());
		}
	}
	~Sound()
	{
		Mix_FreeChunk(data);
		data = NULL;
	}
};

struct Font
{
	TTF_Font* data;
	Font(string filename, int ptsize)
	{
		data = TTF_OpenFont(filename.c_str(), ptsize);
		if (data == NULL)
		{
			throw runtime_error(TTF_GetError());
		}
	}
	~Font()
	{
		TTF_CloseFont(data);
		data = NULL;
	}
};

class AssetManager
{
	Game*							game;
	map<string, Music*>				musics;
	map<string, Sound*>				sounds;
	map<string, Font*>				fonts;

public:
	AssetManager(Game* game);
	~AssetManager();
	void Init();

	Music* GetMusic(string name) const;
	Font* GetFont(string name) const;
	Sound* GetSound(string name) const;
};

