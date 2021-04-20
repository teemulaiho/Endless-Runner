#pragma once

#include "SDL.h"
#include "Easing.h"
#include <cfloat>

class Game;

class Screen
{
	Game*								game;
	SDL_Window*							window;

	const float shakeDuration = 2.0f;
	const float shakeAmount = 10.0f;
	const float shakeSpeed = 100.0f;

	float shakeStart = -FLT_MAX;
	float shakeEnd() { return shakeStart + shakeDuration; }

	static float GetTime() { return ((float)SDL_GetTicks()) * 0.001f; }

public:
	void Init();
	Screen(Game* game);
	~Screen();
	SDL_Window* Window() const { return window; }
	//const int W = 800;
	//const int H = 600;

	int W = 800;
	int H = 600;

	void Shake()
	{
		shakeStart = GetTime();
	}

	int ShakeX()
	{
		float max = static_cast<float>(cos(static_cast<float>(GetTime()) * static_cast<float>(shakeSpeed)) * shakeAmount);
		float min = 0.0f;
		float startTime = shakeStart;
		float endTime = shakeEnd();
		float current = GetTime();

		/*
		@t is the current time (or position) of the tween. This can be seconds or frames, steps, seconds, ms, whatever – as long as the unit is the same as is used for the total time [3].
		@b is the beginning value of the property.
		@c is the change between the beginning and destination value of the property.
		@d is the total time of the tween.
		*/

		float t = current - startTime;
		float b = max;
		float c = -max;
		float d = endTime - startTime;

		if (t > d)return 0;


		float returnValue = Cubic_EaseOut(t, b, c, d);

		return (int)returnValue;
	}

	int ShakeY()
	{
		return 0;
	}


};

