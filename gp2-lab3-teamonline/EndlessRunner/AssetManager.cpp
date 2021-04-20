#include "AssetManager.h"
#include "HeliumMath.h"
#include "Game.h"
#include "Screen.h"
#include <iostream>

using namespace Helium;

AssetManager::AssetManager(Game* game)
{
	this->game = game;
}

AssetManager::~AssetManager()
{
	printf("Destroy Assets\n");

	for (auto music : musics)
	{
		delete music.second;
	}
	musics.clear();

	for (auto font : fonts)
	{
		delete font.second;
	}
	fonts.clear();
}

void AssetManager::Init()
{
	musics["bgmusic"] = new Music("assets/audio/bg3.wav");
	fonts["normal"] = new Font("assets/open-sans/OpenSans-Regular.ttf", 40);
	sounds["hurt"] = new Sound("assets/audio/dusty/spawnnew.wav");
}

Music* AssetManager::GetMusic(string name) const
{
	return musics.find(name)->second;
}

Font* AssetManager::GetFont(string name) const
{
	return fonts.find(name)->second;
}

Sound* AssetManager::GetSound(string name) const
{
	return sounds.find(name)->second;
}
