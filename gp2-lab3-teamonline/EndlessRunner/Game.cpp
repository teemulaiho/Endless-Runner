#include "Game.h"
#include "SDL.h"
#include "Screen.h"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include "CollisionManager.h"
#include "PlayState.h"

#pragma region EnumTranslations

KeyCode TranslateKeyCode(SDL_Keycode code)
{
	switch (code)
	{
	case SDLK_ESCAPE: return KeyCode::ESCAPE; break;
	case SDLK_SPACE: return KeyCode::SPACE; break;
	case SDLK_RETURN: return KeyCode::ENTER; break;
	case SDLK_RETURN2: return KeyCode::ENTER; break;
	case SDLK_a: return KeyCode::A; break;
	case SDLK_b: return KeyCode::B; break;
	case SDLK_c: return KeyCode::C; break;
	case SDLK_d: return KeyCode::D; break;
	case SDLK_e: return KeyCode::E; break;
	case SDLK_f: return KeyCode::F; break;
	case SDLK_g: return KeyCode::G; break;
	case SDLK_h: return KeyCode::H; break;
	case SDLK_i: return KeyCode::I; break;
	case SDLK_j: return KeyCode::J; break;
	case SDLK_k: return KeyCode::K; break;
	case SDLK_l: return KeyCode::L; break;
	case SDLK_m: return KeyCode::M; break;
	case SDLK_n: return KeyCode::N; break;
	case SDLK_o: return KeyCode::O; break;
	case SDLK_p: return KeyCode::P; break;
	case SDLK_q: return KeyCode::Q; break;
	case SDLK_r: return KeyCode::R; break;
	case SDLK_s: return KeyCode::S; break;
	case SDLK_t: return KeyCode::T; break;
	case SDLK_u: return KeyCode::U; break;
	case SDLK_v: return KeyCode::V; break;
	case SDLK_w: return KeyCode::W; break;
	case SDLK_x: return KeyCode::X; break;
	case SDLK_y: return KeyCode::Y; break;
	case SDLK_z: return KeyCode::Z; break;
	case SDLK_LEFT: return KeyCode::LEFT_ARROW; break;
	case SDLK_RIGHT: return KeyCode::RIGHT_ARROW; break;
	case SDLK_UP: return KeyCode::UP_ARROW; break;
	case SDLK_DOWN: return KeyCode::DOWN_ARROW; break;
	default: return KeyCode::INVALID; break;
	}
}

#pragma endregion

Game::Game() : screen(this), assetManager(this)
{
	scoreText = 0;
	guiText = 0;
	livesText = 0;
	renderer = 0;
	state = 0;
}

Game::~Game()
{
	printf("Destroy Game\n");

	for (auto sprite : bgSprites)
		delete sprite;

	bgSprites.clear();

	for (auto sprite : uiSprites)
		delete sprite;

	uiSprites.clear();

	SDL_DestroyRenderer(renderer);
}

void Game::OnKeyDown(const SDL_Event event)
{
}

void Game::ActivateAI()
{
	activate_ai = true;

	std::cout << "AI Mode activated." << std::endl;
}

bool Game::CheckGameObject(Collider& position)
{
	return false;
}

bool Game::CheckCollision(Collider& lhs, Collider& rhs)
{
	return (CollisionManager::CheckCollisionAABB(lhs, rhs));
}

void Game::Init()
{
	fileReader.Read_Configuration("text_configuration.txt", screen.W, screen.H, player_lives, enemy_speed_multiplier);
	screen.Init();
	renderer = SDL_CreateRenderer(screen.Window(), -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assetManager.Init();
	SwitchToState<MenuState>();
	Mix_PlayMusic(assetManager.GetMusic("bgmusic")->data, -1);

	// -----------------------------
	// INITIALIZE BACKGROUND SPRITES
	// -----------------------------

	vector<string> paths;
	for (const auto& entry :
		std::filesystem::directory_iterator("assets/background"))
	{
		if (entry.path().extension().string() == ".png")
		{
			paths.push_back(entry.path().string());
			//sprites.push_back(new Sprite());
		}
	}

	// Sort sprites by file name.
	sort(paths.begin(), paths.end());

	// Add sprites to game.
	for (const auto& path : paths)
	{
		bgSprites.push_back(new Sprite(path, renderer));
	}

	// --------------------------
	// INITIALIZE MENU UI SPRITES
	// --------------------------

	vector<string> path_ui;

	for (const auto& entry :
		std::filesystem::directory_iterator("assets/ui"))
	{
		if (entry.path().extension().string() == ".png")
		{
			path_ui.push_back(entry.path().string());
			//sprites.push_back(new Sprite());
		}
	}

	// Sort sprites by file name.
	sort(path_ui.begin(), path_ui.end());

	// Add sprites to game.
	for (const auto& path : path_ui)
	{
		uiSprites.push_back(new Sprite(path, renderer));
	}

	// ----------------
	// INITIALIZE FONTS
	// ----------------

	scoreText = new GuiFont{
	assetManager.GetFont("normal"),
	{0,0,0},
	"Score:"
	};
	guiText = new GuiFont{
		assetManager.GetFont("normal"),
		{0,0,0},
		"0"
	};
	livesText = new GuiFont{
	assetManager.GetFont("normal"),
	{0,0,0},
	"0"
	};
}

void Game::Render(const GameObject& gameObject)
{
	auto time = ((float)SDL_GetTicks()) * 0.001f;
	auto scroll = (int)(time * (float)gameObject.layer);

	scroll %= screen.W;

	SDL_Rect destination;

	if (gameObject.layer < 301 && !render_menu)						// Render parallax scrolling background images.
	{
		destination.x = -scroll + screen.ShakeX();
		destination.y = 0 + screen.ShakeY();
		destination.w = screen.W;
		destination.h = screen.H;


		SDL_RenderCopy(renderer, gameObject.sprite->texture, 0, &destination);
		destination.x += destination.w;
		SDL_RenderCopy(renderer, gameObject.sprite->texture, 0, &destination);
	}
	else if (gameObject.layer < 301 && render_menu)					// Render parallax scrolling background images.
	{
		destination.x = -scroll/* + screen.ShakeX()*/;
		destination.y = 0/* + screen.ShakeY()*/;
		destination.w = screen.W;
		destination.h = screen.H;


		SDL_RenderCopy(renderer, gameObject.sprite->texture, 0, &destination);
		destination.x += destination.w;
		SDL_RenderCopy(renderer, gameObject.sprite->texture, 0, &destination);
	}

	// Render score.
	if (!render_menu)											// Render score.
	{
		SDL_Surface* surface =
			TTF_RenderText_Solid(guiText->font->data,
				guiText->text.c_str(),
				guiText->textColor);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(
			renderer,
			surface
		);

		destination.x =  surface->w * 2 + screen.W / 9 /*+ screen.ShakeX()*/;
		destination.y = screen.H / 10 /*+ screen.ShakeY()*/;
		destination.w = surface->w;
		destination.h = surface->h;

		SDL_RenderCopy(renderer, texture, NULL, &destination);

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
	}

	// Render "Score: ".
	if (!render_menu)											// Render score.
	{
		SDL_Surface* surface =
			TTF_RenderText_Solid(scoreText->font->data,
				scoreText->text.c_str(),
				scoreText->textColor);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(
			renderer,
			surface
		);

		destination.x = 0/* + screen.ShakeX()*/;
		destination.y = screen.H / 10/* + screen.ShakeY()*/;
		destination.w = surface->w;
		destination.h = surface->h;

		SDL_RenderCopy(renderer, texture, NULL, &destination);

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
	}

	// Render player lives.
	if (!render_menu)											// Render lives.
	{
		SDL_Surface* surface =
			TTF_RenderText_Solid(livesText->font->data,
				livesText->text.c_str(),
				livesText->textColor);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(
			renderer,
			surface
		);

		destination.x = screen.W - screen.W / 9 - surface->w/* + screen.ShakeX()*/;
		destination.y = screen.H / 10/* + screen.ShakeY()*/;

		//player_lives.begin_x = game->GetScreen()->W - game->GetScreen()->W / 10;
		//player_lives.begin_y = game->GetScreen()->H / 10;

		destination.w = surface->w;
		destination.h = surface->h;

		SDL_RenderCopy(renderer, texture, NULL, &destination);

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
	}
}

void Game::Run()
{
	Init();

	/*---- Initialize Game Objects ----*/

	vector<GameObject> gameObjects;

	for (int i = 0; i < (int)bgSprites.size(); i++)
	{
		auto sprite = bgSprites[bgSprites.size() - 1 - i];
		
		Transform transform;
		Collider collider;

		// Initialize game object and give it a layer.
		GameObject gameObject{
			collider, transform, sprite, i* i * 3
		};

		gameObjects.push_back(gameObject);
	}

	// Initialize Menu Game Objects
	for (int i = 0; i < (int)uiSprites.size(); i++)
	{
		auto sprite = uiSprites[i];
		
		Transform transform;
		Collider collider;

		GameObject gameObject{
			collider, transform, sprite, 500
		};

		gameObjects.push_back(gameObject);
	}

	/*--------*/

	/*---- Set Frame Rate ----*/

	const int framesPerSecond = 60;
	const int frameLength = 1000 / framesPerSecond;
	
	/*--------*/

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		auto time = SDL_GetTicks();

		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
			case SDL_KEYDOWN:
				state->Input(event);
			case SDL_MOUSEBUTTONDOWN:
				state->Input(event);
			default:
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xdf, 0xef, 0xff, 0xff);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

		state->Update(frameLength);

		score = static_cast<int> (time / 1000);
		guiText->text = std::to_string(score);
		livesText->text = std::to_string(player_lives);

		for (const auto& gameObject : gameObjects)
			Render(gameObject);

		state->Render(renderer);

		SDL_RenderPresent(renderer);

		int timeElapsed = SDL_GetTicks() - time;
		int timeToWait = frameLength - timeElapsed;

		if (timeToWait > 0)
		{
			SDL_Delay(timeToWait);
		}
	}
}