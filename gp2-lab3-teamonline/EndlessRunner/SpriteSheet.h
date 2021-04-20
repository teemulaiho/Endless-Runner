#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

struct Rect
{
	int x;
	int y;
	int w;
	int h;
};

struct SpriteSheet
{
	std::string							imageFileName;
	std::vector<Rect>					sprites;
};

enum class AnimationLoop {
	Loop,
	PingPong,
	Once,
};

struct Animation
{
	//enum animationLoop {
	//	Loop,
	//	PingPong,
	//	Once
	//};

	bool loop = false;;
	bool loop_once = false;
	bool loop_pingpong = false;

	AnimationLoop						animationloop;
	SpriteSheet*						spriteSheet;
	std::string							spriteSheetLocation;
	std::string							animationType;
	std::string							animationFrameLengthStr;
	int animationFrameLength;
	std::vector<int> frames;

	float elapsedTime = 0.0f;
	unsigned int framecounter = 0;
	int frame = 0;

	bool changeloopdirection = false;
};