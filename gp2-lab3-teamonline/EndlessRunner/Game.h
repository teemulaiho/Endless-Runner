#pragma once

#include "Screen.h"
#include "SDL.h"
#include <string>
#include "SDL_image.h"
#include <vector>
#include "Transform.h"
#include "KeyCode.h"
#include "Player.h"
#include "AssetManager.h"
#include "Collider.h"
#include <cmath>
#include "GuiFont.h"
#include "GameObject.h"
#include "FileReader.h"

using namespace std;

struct Command
{
	KeyCode						key;
	uint32_t					tick;
};

class State
{
protected:
	Game*						game;

public:
	State(Game* game) { this->game = game; }
	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Input(const SDL_Event& event) = 0;
	virtual void Render(SDL_Renderer* renderer) = 0;
};

class Game
{
	SDL_Renderer*				renderer;
	Screen						screen;
	vector<Sprite*>				bgSprites;
	vector<Sprite*>				uiSprites;

	AssetManager				assetManager;
	FileReader					fileReader;

	//Command						cmd;			// Replay Code.
	//vector<Command>				commands;		// Replay Code.

	GuiFont*					scoreText;			// Score text.
	GuiFont*					guiText;			// Score text.
	GuiFont*					livesText;			// Lives text.
	State*						state;

	bool						activate_ai = false;
	bool						render_menu = false;

	void Init();
	void Render(const GameObject& gameObject);
public:
	void Run();
	Game();
	~Game();

	AssetManager* GetAssetManager() { return &assetManager; }
	void OnKeyDown(const SDL_Event event);
	void ActivateAI();
	void ActivateMenuScreen() { render_menu = true; };
	void DeactivateMenuScreen() { render_menu = false; };

	Screen* GetScreen() { return &screen; }
	SDL_Renderer* GetRenderer() { return renderer; }
	
	bool CheckGameObject(Collider& position);
	bool CheckCollision(Collider& lhs, Collider& rhs);

	template<typename T>
	void SwitchToState() 
	{
		if (state != NULL)
		{
			delete state;
			state = NULL;
		}

		state = new T(this);
		state->Init();	
	}

	int							score = 0;
	int							player_lives = 3;
	float						enemy_speed_multiplier = 1.0f;
};

