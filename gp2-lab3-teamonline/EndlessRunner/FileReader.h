#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SpriteSheet.h"

struct FileReader
{
	FileReader();
	~FileReader();

	SpriteSheet* Load_Sprite(std::string filePath);
	Animation* Load_Animation(std::string filePath);

	void Read_Configuration(std::string filePath, int &width, int &height, int &player_lives, float &enemy_speed);
};

void DestroySpriteSheet(SpriteSheet* spritesheet);
void DestroyAnimation(Animation* animation);