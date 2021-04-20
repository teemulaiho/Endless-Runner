#include "FileReader.h"
#include "SDL.h"

FileReader::FileReader()
{
}

FileReader::~FileReader()
{
}

SpriteSheet* FileReader::Load_Sprite(std::string filePath)
{
	SpriteSheet* spriteSheet = new SpriteSheet();

	// Variables for reading from file
	std::string data;
	std::ifstream stream;
	std::string line;

	// Open file.
	stream.open(filePath, std::ios::app);

	// If file did not open, give error.
	if (!stream.is_open())
	{
		std::cout << "Failed to open: " << filePath << std::endl;
	}

	// Get the imageFileName.
	getline(stream, spriteSheet->imageFileName);

	while (getline(stream, line))
	{
		std::stringstream ss(line);

		std::vector<std::string> words;
		std::string word;

		while (ss >> word) words.push_back(word);

		if (words.size() != 4) continue;

		int x = std::stoi(words[0]);
		int y = std::stoi(words[1]);
		int w = std::stoi(words[2]);
		int h = std::stoi(words[3]);

		spriteSheet->sprites.push_back(Rect{
				x,
				y,
				w,
				h,
			});
	}
	stream.close();

	return spriteSheet;
}

Animation* FileReader::Load_Animation(std::string filePath)
{
	Animation* animation = new Animation();

	// Variables for reading from file
	std::string data;
	std::ifstream stream;
	std::string line;

	// Open file.
	stream.open(filePath, std::ios::app);

	// If file did not open, give error.
	if (!stream.is_open())
	{
		std::cout << "Failed to open: " << filePath << std::endl;
	}

	// Get the imageFileName.
	getline(stream, animation->spriteSheetLocation);

	// Get the animationType.
	getline(stream, animation->animationType);

	// Get the animationFrameLength
	getline(stream, animation->animationFrameLengthStr);
	animation->animationFrameLength = stoi(animation->animationFrameLengthStr);

	// Get the frames for animation.
	while (getline(stream, line))
	{
		std::stringstream ss(line);

		std::vector<std::string> words;
		std::string word;

		while (ss >> word) words.push_back(word);

		for (unsigned int i = 0; i < words.size(); i++)
		{
			animation->frames.push_back(std::stoi(words.at(i)));
		}
	}

	stream.close();

	return animation;
}

void FileReader::Read_Configuration(std::string filePath, int& width, int& height, int& player_lives, float& enemy_speed_multiplier)
{
	// Configuration variables
	// configuration[0] = width
	// configuration[1] = height
	// configuration[2] = player lives
	// configuration[3] = enemy speed increas
	std::vector<int> configuration;

	// Variables for reading from file
	std::string data;
	std::ifstream stream;
	std::string line;

	// Open file.
	stream.open(filePath, std::ios::app);

	// If file did not open, give error.
	if (!stream.is_open())
	{
		std::cout << "Failed to open: " << filePath << std::endl;
	}

	// Get the frames for animation.
	while (getline(stream, line))
	{
		std::stringstream ss(line);

		std::vector<std::string> words;
		std::string word;

		while (ss >> word) words.push_back(word);

		for (unsigned int i = 0; i < words.size(); i++)
		{
			configuration.push_back(std::stoi(words.at(i)));
		}
	}

	stream.close();

	width = configuration.at(0);
	height = configuration.at(1);
	player_lives = configuration.at(2);
	enemy_speed_multiplier = 1.0f + (static_cast<float>(configuration.at(3))) / 100;					
}

void DestroySpriteSheet(SpriteSheet* spritesheet)
{
	if (spritesheet == nullptr)
	{
		return;
	}

	delete spritesheet;
}

void DestroyAnimation(Animation* animation)
{
	if (animation == nullptr)
	{
		return;
	}

	delete animation;
}
