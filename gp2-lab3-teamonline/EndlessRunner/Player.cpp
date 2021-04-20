#include "Player.h"
#include "limits.h"

Player::Player()
{
	animation = 0;
	jumpanimation = 0;
	hurtanimation = 0;
	deathanimation = 0;

	spritesheet = 0;
	jumpspritesheet = 0;
	hurtspritesheet = 0;
	deathspritesheet = 0;

	surface = 0;
	jumpsurface = 0;
	hurtsurface = 0;
	deathsurface = 0;

	texture = 0;
	jumptexture = 0;
	hurttexture = 0;
	deathtexture = 0;
}

Player::~Player()
{
	printf("Destroy Player\n");
}

void Player::Initialize(std::string filePath, SDL_Renderer* renderer)
{
	// Set player starting position.
	transform.SetPosition(static_cast<float>(player_begin_x), static_cast<float>(player_begin_y));

	// Load Run Animation and SpriteSheet.
	animation = filereader.Load_Animation(filePath);
	spritesheet = filereader.Load_Sprite(animation->spriteSheetLocation.c_str());
	animation->spriteSheet = spritesheet;

	surface = IMG_Load(spritesheet->imageFileName.c_str());

	if (surface == NULL)
	{
		printf("Could not load file from: ");
		std::cout << spritesheet->imageFileName.c_str() << std::endl;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	// Load Jump Animation and SpriteSheet.
	jumpanimation = filereader.Load_Animation("text_player_jump_animation.txt");
	jumpspritesheet = filereader.Load_Sprite(jumpanimation->spriteSheetLocation.c_str());
	jumpanimation->spriteSheet = jumpspritesheet;

	jumpsurface = IMG_Load(jumpspritesheet->imageFileName.c_str());

	if (jumpsurface == NULL)
	{
		printf("Could not load file from: ");
		std::cout << jumpspritesheet->imageFileName.c_str() << std::endl;
	}

	jumptexture = SDL_CreateTextureFromSurface(renderer, jumpsurface);

	// Load Hurt Animation and SpriteSheet.
	hurtanimation = filereader.Load_Animation("text_player_hurt_animation.txt");
	hurtspritesheet = filereader.Load_Sprite(hurtanimation->spriteSheetLocation.c_str());
	hurtanimation->spriteSheet = hurtspritesheet;

	hurtsurface = IMG_Load(hurtspritesheet->imageFileName.c_str());

	if (hurtsurface == NULL)
	{
		printf("Could not load file from: ");
		std::cout << hurtspritesheet->imageFileName.c_str() << std::endl;
	}

	hurttexture = SDL_CreateTextureFromSurface(renderer, hurtsurface);

	// Load Death Animation and SpriteSheet.
	deathanimation = filereader.Load_Animation("text_player_death_animation.txt");
	deathspritesheet = filereader.Load_Sprite(deathanimation->spriteSheetLocation.c_str());
	deathanimation->spriteSheet = deathspritesheet;

	deathsurface = IMG_Load(deathspritesheet->imageFileName.c_str());

	if (deathsurface == NULL)
	{
		printf("Could not load file from: ");
		std::cout << deathspritesheet->imageFileName.c_str() << std::endl;
	}

	deathtexture = SDL_CreateTextureFromSurface(renderer, deathsurface);

	// Set collider size after getting spritesheet details.
	collider.rect.w = animation->spriteSheet->sprites.at(0).w;
	collider.rect.h = animation->spriteSheet->sprites.at(0).h;

	// Save data to vectors.
	{
		animations.push_back(animation);
		spritesheets.push_back(spritesheet);
		surfaces.push_back(surface);
		textures.push_back(texture);

		animations.push_back(jumpanimation);
		spritesheets.push_back(jumpspritesheet);
		surfaces.push_back(jumpsurface);
		textures.push_back(jumptexture);

		animations.push_back(hurtanimation);
		spritesheets.push_back(hurtspritesheet);
		surfaces.push_back(hurtsurface);
		textures.push_back(hurttexture);
	}
}

void Player::Animate(float dt)
{
	// Reset jumpanimation frame counter if animation has played once and is able to play again.
	if (player_grounded)
		animations.at(1)->framecounter = 0;

	AnimatePlay(animation_index, dt);
}

void Player::AnimatePlay(int index, float dt)
{
	// Add time to elapsedTime.
	animations.at(index)->elapsedTime += dt;

	// Check if it is time to change frame.
	if (animations.at(index)->elapsedTime > animations.at(index)->animationFrameLength)
	{
		animations.at(index)->elapsedTime = 0.0f;
		animations.at(index)->framecounter++;

		if (animations.at(index)->framecounter >= INT_MAX)
			animations.at(index)->framecounter = 0;
	}
	if (animations.at(index)->animationType == "Loop")
	{
		animations.at(index)->frame = animations.at(index)->framecounter % animations.at(index)->frames.size();
	}
	else if (animations.at(index)->animationType == "Once")
	{
		if (animations.at(index)->framecounter >= animations.at(index)->frames.size())
		{
			animations.at(index)->frame = animations.at(index)->frames.size() - 1;

			if (animations.at(index)->spriteSheetLocation == "text_player_sprite_jump.txt")
			{
				player_grounded = true;
				player_running = true;
				animation_index = 0;
			}
		}
		else
			animations.at(index)->frame = animations.at(index)->framecounter % animations.at(index)->frames.size();
	}
	else if (animations.at(index)->animationType == "Pingpong")
	{
		int animationframesize = animations.at(index)->frames.size() - 1;
		int framesizedoubled = animationframesize * 2;
		int loop = animations.at(index)->framecounter % framesizedoubled;

		if (loop <= animationframesize)
		{
			animations.at(index)->frame = loop;
		}
		else
		{
			animations.at(index)->frame = framesizedoubled - loop;
		}
	}
	else
		std::cout << "NONE" << std::endl;




		//// Add time to elapsedTime.
		//animation->elapsedTime += dt;

		//// Check if it is time to change frame.
		//if (animation->elapsedTime > animation->animationFrameLength)
		//{
		//	animation->elapsedTime = 0.0f;
		//	animation->framecounter++;

		//	if (animation->framecounter >= INT_MAX)
		//		animation->framecounter = 0;
		//}

		//if (animation->animationType == "Loop")
		//{
		//	animation->frame = animation->framecounter % animation->frames.size();
		//}
		//else if (animation->animationType == "Once")
		//{
		//	if (animation->framecounter >= animation->frames.size())
		//	{
		//		animation->frame = animation->frames.size() - 1;
		//	}
		//	else
		//		animation->frame = animation->framecounter % animation->frames.size();
		//}
		//else if (animation->animationType == "Pingpong")
		//{
		//	int animationframesize = animation->frames.size() - 1;
		//	int framesizedoubled = animationframesize * 2;
		//	int loop = animation->framecounter % framesizedoubled;

		//	if (loop <= animationframesize)
		//	{
		//		animation->frame = loop;
		//	}
		//	else
		//	{
		//		animation->frame = framesizedoubled - loop;
		//	}
		//}
		//else
		//	std::cout << "NONE" << std::endl;
}

void Player::Move()
{
	if (is_flying_enemy)
	{
		// Set location of sprite on screen.
		float seconds = ((float)SDL_GetTicks()) / 1000.0f;
		//transform.position.x = transform.position.x + (int)(cos(seconds) * 30);
		//transform.position.y = transform.position.y + (int)(sin(seconds) * 30);

		transform.position.x = transform.position.x + (int)(cos(seconds) * 2);
		transform.position.y = transform.position.y + (int)(sin(seconds) * 2);
	}

	if (transform.position.y < /*500*/ player_begin_y && !is_enemy)
	{
		player_grounded = false;
		//player_running = true;
	}

	if (jumpanimation->frame > 0 && !player_collided_enemy)
	{
		if (transform.position.y < /*500*/ player_begin_y)
			transform.ChangePosition(0, 4.0f);
	}
	else if (!player_grounded && !player_collided_enemy)
	{
		transform.ChangePosition(0, -4.0f);
	}

	collider.SetPosition(static_cast<int>(transform.GetPosition().x), static_cast<int>(transform.GetPosition().y));
}

void Player::ResetAnimation(Animation* animation)
{
	animation->framecounter = 0;
	animation->elapsedTime = 0;
}

void Player::Update(float dt)
{
	if (player_lives == 0)
		animation_index = 3;
	Animate(dt);
	Move();
}

void Player::Render(SDL_Renderer* renderer)
{
	// Set frame to show for Game Object.
	SDL_Rect test_source_test;		
	SDL_Rect test_destination_test;
	SDL_Texture* render_texture = texture;
	
	animations.at(animation_index)->spriteSheet->sprites.at(animations.at(animation_index)->frame).x;

	test_source_test.x = animations.at(animation_index)->spriteSheet->sprites.at(animations.at(animation_index)->frame).x;
	test_source_test.y = animations.at(animation_index)->spriteSheet->sprites.at(animations.at(animation_index)->frame).y;
	test_source_test.w = animations.at(animation_index)->spriteSheet->sprites.at(animations.at(animation_index)->frame).w;
	test_source_test.h = animations.at(animation_index)->spriteSheet->sprites.at(animations.at(animation_index)->frame).h;

	// Set location of sprite on screen.
	test_destination_test.x = static_cast<int>(transform.position.x);
	test_destination_test.y = static_cast<int>(transform.position.y);
	// Set size of sprite on sreen.
	test_destination_test.w = test_source_test.w * 2;
	test_destination_test.h = test_source_test.h * 2;

	render_texture = textures.at(animation_index);

	SDL_RenderCopy(renderer, render_texture, &test_source_test, &test_destination_test);
}

void Player::OnKeyDown(const SDL_Event event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_SPACE:
		{
			Jump();
		}
		default:
			break;
		}
	}
}

void Player::Jump()
{
	player_running = false;
	player_grounded = false;
	animation_index = 1;
}

