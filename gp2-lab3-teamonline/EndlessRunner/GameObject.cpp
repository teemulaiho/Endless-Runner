#include "GameObject.h"

GameObject::~GameObject()
{
}

void GameObject::Initialize(std::string filePath, SDL_Renderer* renderer)
{
	// Set GameObject starting position.
	transform.SetPosition(static_cast<float>(begin_x), static_cast<float>(begin_y));

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

	// Set collider size after getting spritesheet details.
	collider.rect.w = animation->spriteSheet->sprites.at(0).w;
	collider.rect.h = animation->spriteSheet->sprites.at(0).h;

	// Save data to vectors.
	{
		animations.push_back(animation);
		spritesheets.push_back(spritesheet);
		surfaces.push_back(surface);
		textures.push_back(texture);
	}
}

void GameObject::Update(float dt)
{
	Animate(dt);
	Move();
}

void GameObject::Move()
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

	if (is_enemy)
	{
		transform.ChangePosition(-1 * enemyspeed, 0);

		if (transform.position.y < begin_y && !is_flying_enemy)
			transform.ChangePosition(0.0f, 1.0f);

		if (transform.position.x <= -36)
		{
			transform.SetPosition(static_cast<float>(begin_x + 36), static_cast<float>(begin_y));

			if (enemyspeed < 5.0f)
				enemyspeed = enemyspeed * enemy_speed_multiplier;
		}
	}

	collider.SetPosition(static_cast<int>(transform.GetPosition().x), static_cast<int>(transform.GetPosition().y));
}

void GameObject::Animate(float dt)
{
	AnimatePlay(animation_index, dt);
}

void GameObject::AnimatePlay(int index, float dt)
{

	if (animations.size() > 0)
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
	}
	else
		std::cout << "No animations loaded for GameObject. Remember to initialize GameObject." << std::endl;

}

void GameObject::Render(SDL_Renderer* renderer)
{
	// Set frame to show for Game Object.
	SDL_Rect source;
	SDL_Rect destination;
	SDL_Texture* render_texture = texture;

	//// Set frame to show for Game Object.
	//SDL_Rect source;
	//SDL_Rect destination;
	//SDL_Texture* render_texture = texture;

	source.x = animations.at(animation_index)->spriteSheet->sprites.at(animations.at(animation_index)->frame).x;
	source.y = animations.at(animation_index)->spriteSheet->sprites.at(animations.at(animation_index)->frame).y;
	source.w = animations.at(animation_index)->spriteSheet->sprites.at(animations.at(animation_index)->frame).w;
	source.h = animations.at(animation_index)->spriteSheet->sprites.at(animations.at(animation_index)->frame).h;

	// Set location of sprite on screen.
	destination.x = static_cast<int>(transform.position.x);
	destination.y = static_cast<int>(transform.position.y);
	// Set size of sprite on sreen.
	destination.w = source.w * 2;
	destination.h = source.h * 2;

	render_texture = textures.at(animation_index);

	SDL_RenderCopy(renderer, render_texture, &source, &destination);
}
