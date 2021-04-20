#include "PlayState.h"

PlayState::PlayState(Game* game) : State(game)
{
	this->assetManager = game->GetAssetManager();
}

PlayState::~PlayState()
{
}

void PlayState::Init()
{
	hurt = assetManager->GetSound("hurt")->data;

	std::cout << "Initializing PlayState" << std::endl;
	game->DeactivateMenuScreen();

	// Initialize GameObject player.
	player.player_begin_y = (int)((float)game->GetScreen()->H * 0.83f);
	player.player_lives = game->player_lives;
	player.Initialize("text_player_run_animation.txt", game->GetRenderer());

	// Initialize GameObject player_lives.
	player_lives.Initialize("text_player_run_animation.txt", game->GetRenderer());
	player_lives.begin_x = game->GetScreen()->W - game->GetScreen()->W / 10;
	player_lives.begin_y = game->GetScreen()->H / 10 - player_lives.animation->spriteSheet->sprites.at(0).h / 4;
	player_lives.transform.SetPosition(static_cast<float>(player_lives.begin_x), static_cast<float>(player_lives.begin_y));

	// Initalize GameObject opossum.
	opossum.Initialize("text_opossum_run_animation_loop.txt", game->GetRenderer());
	opossum.begin_x = game->GetScreen()->W;
	opossum.begin_y = (int) ((float)game->GetScreen()->H * 0.83f);
	opossum.transform.SetPosition(static_cast<float>(opossum.begin_x), static_cast<float>(opossum.begin_y));
	opossum.is_enemy = true;
	opossum.enemy_speed_multiplier = game->enemy_speed_multiplier;

	// Initialize GameObject frog.
	frog.Initialize("text_frog_run_animation_loop.txt", game->GetRenderer());
	frog.begin_x = game->GetScreen()->W;
	frog.begin_y = (int)((float)game->GetScreen()->H * 0.83f);
	frog.transform.SetPosition(static_cast<float>(frog.begin_x + 400), static_cast<float>(frog.begin_y));
	frog.is_enemy = true;
	frog.enemy_speed_multiplier = game->enemy_speed_multiplier;

	// Initialize GameObject eagle.
	eagle.Initialize("text_eagle_animation_test.txt", game->GetRenderer());
	eagle.begin_x = game->GetScreen()->W;
	eagle.begin_y = (int)((float)game->GetScreen()->H * 0.67f);
	//eagle.begin_y = 400;
	eagle.transform.SetPosition(static_cast<float>(300), static_cast<float>(eagle.begin_y));
	eagle.is_enemy = true;
	eagle.is_flying_enemy = true;
	eagle.enemy_speed_multiplier = game->enemy_speed_multiplier;
}

void PlayState::Update(float frameLength)
{
	if (game->player_lives <= 0)				// If player lives run out, switch to MenuState.
	{
		game->player_lives = player.player_lives;
		game->score = 0;
		game->SwitchToState<GameOverState>();
		return;
	}

	if (game->player_lives > 0)					// If player has lives left, keep updating PlayState.
	{
		player.Update(frameLength);
		player_lives.Update(frameLength);
		opossum.Update(frameLength);
		frog.Update(frameLength);
		eagle.Update(frameLength);

		// Check for collision between enemies. If collision, Resolve by moving left hand side collider.
		if (game->CheckCollision(opossum.collider, frog.collider))
		{
			if (opossum.transform.position.x > frog.transform.position.x)
			{
				opossum.transform.ChangePosition(-2.0f, 0.0f);
			}
			else
				opossum.transform.ChangePosition(-2.0f, 0.0f);
		}


		// Check for collision between enemies and player.
		if (game->CheckCollision(opossum.collider, player.collider)		||
			game->CheckCollision(frog.collider, player.collider)		||
			game->CheckCollision(eagle.collider, player.collider))
		{
			// Check if player has more than 0 lives left, and if they aren't already colliding with an enemy.
			if (game->player_lives > 0 && !player.player_collided_enemy)
			{
				game->player_lives--;

				string soundName = "hurt";

				Mix_PlayChannel(-1, assetManager->GetSound(soundName)->data, 0);
			}
	

			player.player_collided_enemy = true;
			game->GetScreen()->Shake();

			if (player.player_lives > 0)
				player.animation_index = 2;

			player.ResetAnimation(player.jumpanimation);
			//player2.player_grounded = true;
		}
		else if (player.animation_index != 1)
		{
			if (player.player_lives > 0)
				player.animation_index = 0;

			player.player_collided_enemy = false;
		}
	}
}

void PlayState::Input(const SDL_Event& event)
{
	player.OnKeyDown(event);
	switch (event.type)
	{
	case SDL_QUIT:
			return;
	case SDL_KEYDOWN :
		switch (event.key.keysym.sym)
		{
		case SDLK_p:
		{
			game->SwitchToState<MenuState>();
		}
		case SDLK_SPACE:
		{
			game->OnKeyDown(event);
		}
		default:
			break;
		}
	}
}

void PlayState::Render(SDL_Renderer* renderer)
{
	opossum.Render(renderer);
	frog.Render(renderer);
	eagle.Render(renderer);
	player.Render(renderer);
	player_lives.Render(renderer);
}

MenuState::MenuState(Game* game) : State(game)
{
	this->assetManager = game->GetAssetManager();
}

MenuState::~MenuState()
{
}

void MenuState::Init()
{
	std::cout << "Initializing MenuState" << std::endl;
	game->ActivateMenuScreen();

	// Initialize "Settings" -menu button.
	settingsMenu.Initialize("text_menu_settings_animation.txt", game->GetRenderer());
	settingsMenu.begin_x = game->GetScreen()->W / 2 - settingsMenu.animation->spriteSheet->sprites.at(0).w;
	settingsMenu.begin_y = game->GetScreen()->H / 2 - settingsMenu.animation->spriteSheet->sprites.at(0).h;
	settingsMenu.collider.SetSize(settingsMenu.animation->spriteSheet->sprites.at(0).w, settingsMenu.animation->spriteSheet->sprites.at(0).h);
	
	settingsMenu.transform.SetPosition((float)settingsMenu.begin_x, (float)settingsMenu.begin_y);
	settingsMenu.collider.SetPosition(static_cast<int>(settingsMenu.transform.GetPosition().x), static_cast<int>(settingsMenu.transform.GetPosition().y));

	settingsMenu.is_menu = true;


	// Initialize "Play Game" -menu button.
	playMenu.Initialize("text_menu_playgame_animation.txt", game->GetRenderer());
	playMenu.begin_x = game->GetScreen()->W / 2 - playMenu.animation->spriteSheet->sprites.at(0).w;
	playMenu.begin_y = 0 /*game->GetScreen()->H / 4*/;
	playMenu.collider.SetSize(playMenu.animation->spriteSheet->sprites.at(0).w, playMenu.animation->spriteSheet->sprites.at(0).h);

	playMenu.transform.SetPosition((float)playMenu.begin_x, (float)playMenu.begin_y);
	playMenu.collider.SetPosition(static_cast<int>(playMenu.transform.GetPosition().x), static_cast<int>(playMenu.transform.GetPosition().y));

	playMenu.is_menu = true;

	// Initialize "Quit Game" -menu button.
	quitMenu.Initialize("text_menu_quitgame_animation.txt", game->GetRenderer());
	quitMenu.begin_x = game->GetScreen()->W / 2 - quitMenu.animation->spriteSheet->sprites.at(0).w;
	quitMenu.begin_y = game->GetScreen()->H - quitMenu.animation->spriteSheet->sprites.at(0).h * 2;
	quitMenu.collider.SetSize(quitMenu.animation->spriteSheet->sprites.at(0).w, quitMenu.animation->spriteSheet->sprites.at(0).h);

	quitMenu.transform.SetPosition((float)quitMenu.begin_x, (float)quitMenu.begin_y);
	quitMenu.collider.SetPosition(static_cast<int>(quitMenu.transform.GetPosition().x), static_cast<int>(quitMenu.transform.GetPosition().y));

	quitMenu.is_menu = true;

}

void MenuState::Update(float deltaTime)
{
	// If mouse hover over settings boxes.
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		mouseCollider.SetPosition(x, y);
		mouseCollider.SetSize(32, 32);

		settingsMenu.hovered_over = game->CheckCollision(settingsMenu.collider, mouseCollider);

		if (settingsMenu.hovered_over)
		{
			playMenu.hovered_over = false;
			quitMenu.hovered_over = false;
			playMenu.menu_shake_times = 1;

			if (settingsMenu.menu_shake_times > 0)
				game->GetScreen()->Shake();

			settingsMenu.menu_shake_times--;
		}
		else if (!settingsMenu.hovered_over)
			settingsMenu.menu_shake_times = 1;

		playMenu.hovered_over = game->CheckCollision(playMenu.collider, mouseCollider);

		if (playMenu.hovered_over)
		{
			settingsMenu.hovered_over = false;
			quitMenu.hovered_over = false;
			settingsMenu.menu_shake_times = 1;

			if (playMenu.menu_shake_times > 0)
				game->GetScreen()->Shake();

			playMenu.menu_shake_times--;
		}
		else if (!playMenu.hovered_over)
			playMenu.menu_shake_times = 1;

		quitMenu.hovered_over = game->CheckCollision(quitMenu.collider, mouseCollider);

		if (quitMenu.hovered_over)
		{
			settingsMenu.hovered_over = false;
			playMenu.hovered_over = false;
			settingsMenu.menu_shake_times = 1;

			if (quitMenu.menu_shake_times > 0)
				game->GetScreen()->Shake();

			quitMenu.menu_shake_times--;
		}
		else if (!quitMenu.hovered_over)
			quitMenu.menu_shake_times = 1;
	}

	if (settingsMenu.hovered_over)
	{
		settingsMenu.transform.ChangePosition(
			static_cast<float>(game->GetScreen()->ShakeX()),
			static_cast<float>(game->GetScreen()->ShakeY())
		);
	}

	settingsMenu.collider.SetPosition(static_cast<int>(settingsMenu.transform.GetPosition().x), static_cast<int>(settingsMenu.transform.GetPosition().y));

	if (playMenu.hovered_over)
	{
		playMenu.transform.ChangePosition(
			static_cast<float>(game->GetScreen()->ShakeX()),
			static_cast<float>(game->GetScreen()->ShakeY())
		);
	}

	playMenu.collider.SetPosition(static_cast<int>(playMenu.transform.GetPosition().x), static_cast<int>(playMenu.transform.GetPosition().y));

	if (quitMenu.hovered_over)
	{
		quitMenu.transform.ChangePosition(
			static_cast<float>(game->GetScreen()->ShakeX()),
			static_cast<float>(game->GetScreen()->ShakeY())
		);
	}

	quitMenu.collider.SetPosition(static_cast<int>(quitMenu.transform.GetPosition().x), static_cast<int>(quitMenu.transform.GetPosition().y));
}

void MenuState::Input(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_QUIT:
		return;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_o:
		{
			game->SwitchToState<PlayState>();
		}
		default:
			break;
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		mouseCollider.SetPosition(x, y);
		mouseCollider.SetSize(32, 32);

		settingsMenu.clicked = game->CheckCollision(settingsMenu.collider, mouseCollider);

		if (settingsMenu.clicked)
		{
			playMenu.clicked = false;
		}

		playMenu.clicked = game->CheckCollision(playMenu.collider, mouseCollider);

		if (playMenu.clicked)
		{
			settingsMenu.clicked = false;
			game->SwitchToState<PlayState>();
		}

		quitMenu.clicked = game->CheckCollision(quitMenu.collider, mouseCollider);

		if (quitMenu.clicked)
		{
			std::cout << "Mouse click on quitMenu" << std::endl;
		}
	}
	default:
		break;
	}
}

void MenuState::Render(SDL_Renderer* renderer)
{
	settingsMenu.Render(renderer);
	playMenu.Render(renderer);
	quitMenu.Render(renderer);
}

GameOverState::GameOverState(Game* game) : State(game)
{
	this->assetManager = game->GetAssetManager();
}

GameOverState::~GameOverState()
{
}

void GameOverState::Init()
{
	std::cout << "Initializing GameOverState" << std::endl;
	game->ActivateMenuScreen();

	// Initialize "GameOver" -Game Object
	gameOver.Initialize("text_menu_gameover_animation.txt", game->GetRenderer());
	gameOver.begin_x = game->GetScreen()->W / 2 - gameOver.animation->spriteSheet->sprites.at(0).w;
	gameOver.begin_y = 0 /*game->GetScreen()->H / 2*/;
	gameOver.collider.SetSize(gameOver.animation->spriteSheet->sprites.at(0).w, gameOver.animation->spriteSheet->sprites.at(0).h);

	gameOver.transform.SetPosition((float)gameOver.begin_x, (float)gameOver.begin_y);
	gameOver.collider.SetPosition(static_cast<int>(gameOver.transform.GetPosition().x), static_cast<int>(gameOver.transform.GetPosition().y));

	gameOver.is_menu = true;

	// Initialize "Play Game" -menu button.
	playMenu.Initialize("text_menu_playgame_animation.txt", game->GetRenderer());
	playMenu.begin_x = game->GetScreen()->W / 2 - playMenu.animation->spriteSheet->sprites.at(0).w;
	playMenu.begin_y = game->GetScreen()->H / 2 - playMenu.animation->spriteSheet->sprites.at(0).h;
	playMenu.collider.SetSize(playMenu.animation->spriteSheet->sprites.at(0).w, playMenu.animation->spriteSheet->sprites.at(0).h);

	playMenu.transform.SetPosition((float)playMenu.begin_x, (float)playMenu.begin_y);
	playMenu.collider.SetPosition(static_cast<int>(playMenu.transform.GetPosition().x), static_cast<int>(playMenu.transform.GetPosition().y));

	playMenu.is_menu = true;
}

void GameOverState::Update(float deltaTime)
{
	// If mouse hover over settings boxes.
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		mouseCollider.SetPosition(x, y);
		mouseCollider.SetSize(32, 32);

		gameOver.hovered_over = game->CheckCollision(gameOver.collider, mouseCollider);

		if (gameOver.hovered_over)
		{
			playMenu.hovered_over = false;
			playMenu.menu_shake_times = 1;

			if (gameOver.menu_shake_times > 0)
				game->GetScreen()->Shake();

			gameOver.menu_shake_times--;
		}
		else if (!gameOver.hovered_over)
			gameOver.menu_shake_times = 1;

		playMenu.hovered_over = game->CheckCollision(playMenu.collider, mouseCollider);

		if (playMenu.hovered_over)
		{
			gameOver.hovered_over = false;
			gameOver.menu_shake_times = 1;

			if (playMenu.menu_shake_times > 0)
				game->GetScreen()->Shake();

			playMenu.menu_shake_times--;
		}
		else if (!playMenu.hovered_over)
			playMenu.menu_shake_times = 1;
	}

	if (playMenu.hovered_over)
	{
		playMenu.transform.ChangePosition(
			static_cast<float>(game->GetScreen()->ShakeX()),
			static_cast<float>(game->GetScreen()->ShakeY())
		);
	}

	playMenu.collider.SetPosition(static_cast<int>(playMenu.transform.GetPosition().x), static_cast<int>(playMenu.transform.GetPosition().y));

	if (gameOver.hovered_over)
	{
		gameOver.transform.ChangePosition(
			static_cast<float>(game->GetScreen()->ShakeX()),
			static_cast<float>(game->GetScreen()->ShakeY())
		);
	}

	gameOver.collider.SetPosition(static_cast<int>(gameOver.transform.GetPosition().x), static_cast<int>(gameOver.transform.GetPosition().y));
}

void GameOverState::Input(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_QUIT:
		return;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_o:
		{
			game->SwitchToState<PlayState>();
		}
		default:
			break;
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		mouseCollider.SetPosition(x, y);
		mouseCollider.SetSize(32, 32);

		gameOver.clicked = game->CheckCollision(gameOver.collider, mouseCollider);

		if (gameOver.clicked)
		{
			playMenu.clicked = false;
			std::cout << "Mouse click on gameOver" << std::endl;
		}

		playMenu.clicked = game->CheckCollision(playMenu.collider, mouseCollider);

		if (playMenu.clicked)
		{
			gameOver.clicked = false;
			std::cout << "Mouse click on playMenu" << std::endl;

			game->SwitchToState<PlayState>();
		}
	}
	default:
		break;
	}
}

void GameOverState::Render(SDL_Renderer* renderer)
{
	gameOver.Render(renderer);
	playMenu.Render(renderer);
}
