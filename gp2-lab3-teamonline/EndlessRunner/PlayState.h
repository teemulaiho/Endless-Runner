#pragma once

#include "Game.h"
#include "SDL_mixer.h"

class PlayState :
	public State
{
	AssetManager*						assetManager;

	Player								player;
	GameObject							player_lives;
	GameObject							opossum;
	GameObject							frog;
	GameObject							eagle;

	Mix_Chunk*							hurt = 0;;

public:
	PlayState(Game* game);
	~PlayState();

	// Inherited via State
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Input(const SDL_Event& event) override;
	virtual void Render(SDL_Renderer* renderer) override;
};

class MenuState :
	public State
{
	AssetManager*						assetManager;
	Collider							mouseCollider;

	GameObject							settingsMenu;
	GameObject							playMenu;
	GameObject							quitMenu;

public:
	MenuState(Game* game);
	~MenuState();

	// Inherited via State
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Input(const SDL_Event& event) override;
	virtual void Render(SDL_Renderer* renderer) override;
};

class GameOverState :
	public State
{
	AssetManager*						assetManager;
	Collider							mouseCollider;

	GameObject							playMenu;
	GameObject							gameOver;

public:
	GameOverState(Game* game);
	~GameOverState();

	// Inherited via State
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Input(const SDL_Event& event) override;
	virtual void Render(SDL_Renderer* renderer) override;
};