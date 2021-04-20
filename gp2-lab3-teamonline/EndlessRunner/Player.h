#pragma once
#include "SpriteSheet.h"
#include "FileReader.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Screen.h"
#include "Transform.h"
#include "Collider.h"
#include "KeyCode.h"
#include "GameObject.h"

struct Player : 
	public GameObject 
{
	// FileReader to read Animations and SpriteSheet
	FileReader							filereader;

	Animation*							animation;
	SpriteSheet*						spritesheet;
	SDL_Surface*						surface;
	SDL_Texture*						texture;

	Animation*							jumpanimation;
	SpriteSheet*						jumpspritesheet;
	SDL_Surface*						jumpsurface;
	SDL_Texture*						jumptexture;
	
	Animation*							hurtanimation;
	SpriteSheet*						hurtspritesheet;
	SDL_Surface*						hurtsurface;
	SDL_Texture*						hurttexture;
		
	Animation*							deathanimation;
	SpriteSheet*						deathspritesheet;
	SDL_Surface*						deathsurface;
	SDL_Texture*						deathtexture;

	std::vector<SpriteSheet*>			spritesheets;
	std::vector<Animation*>				animations;
	std::vector<SDL_Surface*>			surfaces;
	std::vector<SDL_Texture*>			textures;

	int animation_index = 0;

	int player_begin_x = 0;
	int player_begin_y = 500;

	int player_lives = 3;

	bool player_running = false;
	bool player_grounded = true;
	bool player_collided_enemy = false;

	bool is_enemy = false;
	bool is_flying_enemy = false;

	Player();
	~Player();

	void Initialize(std::string filePath, SDL_Renderer* renderer);
	void Animate(float dt);
	void AnimatePlay(int index, float dt);
	void Move();
	void ResetAnimation(Animation* animation);
	void Update(float dt);
	void Render(SDL_Renderer* renderer);
	void OnKeyDown(const SDL_Event event);
	void Jump();
};