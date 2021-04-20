#pragma once
#include "Collider.h"
#include "Transform.h"
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include "FileReader.h"

using namespace std;

struct Sprite
{
	SDL_Surface*						surface;
	SDL_Texture*						texture;

	Sprite(string filename, SDL_Renderer* renderer)
	{
		surface = IMG_Load(filename.c_str());
		texture = SDL_CreateTextureFromSurface(renderer, surface);
	}
	~Sprite() 
	{
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}
};


class GameObject
{
	// Game object main properties:
	// - it's own FileReader to read animations.
	// - it's own animation (read from file).
	// - it's own spritesheet.
	// - it's own collider.
	// - it's own identity (is background, is enemy, is obstacle, is player, etc.).
	// - it's own position (x, y).

public:

	FileReader							filereader;
	Animation*							animation;
	SpriteSheet*						spritesheet;
	SDL_Surface*						surface;
	SDL_Texture*						texture;
	int									animation_index = 0;

	std::vector<SpriteSheet*>			spritesheets;
	std::vector<Animation*>				animations;
	std::vector<SDL_Surface*>			surfaces;
	std::vector<SDL_Texture*>			textures;

	Collider							collider;
	Transform							transform;
	Sprite*								sprite;
	int									layer;

	int									begin_x = 0;
	int									begin_y = 500;

	bool								is_enemy = false;
	bool								is_flying_enemy = false;
	float								enemyspeed = 2.0f;
	float								enemy_speed_multiplier = 1.2f;

	bool								is_menu = false;
	bool								hovered_over = false;
	bool								clicked = false;
	int									menu_shake_times = 1;

	GameObject()
	{
		animation = 0;
		spritesheet = 0;
		surface = 0;
		texture = 0;

		sprite = 0;
		layer = 0;
	}
	GameObject(Collider _collider, Transform _transform, Sprite* _sprite, int _layer)
	{
		collider = _collider;
		transform = _transform;
		sprite = _sprite;
		layer = _layer;
	}
	~GameObject();

	void Initialize(std::string filePath, SDL_Renderer* renderer);
	void Update(float dt);
	void Move();
	void Animate(float dt);
	void AnimatePlay(int index, float dt);
	void Render(SDL_Renderer* renderer);
};