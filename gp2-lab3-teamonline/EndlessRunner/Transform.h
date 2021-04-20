// Code From: Jerry Jonsson / Uppsala University / Game Programming 1: Computer Games in 2D | 5.0 hp | 5SD804

#pragma once

#include "Vector2.h"

struct Transform
{
	Transform();
	Transform(const Transform& rhs);

	void SetPosition(const float x, const float y);
	void SetRotation(const float rotation);
	void SetScale(const float x, const float y);

	void ChangePosition(const float x, const float y);
	void ChangeRotation(const float rotation);
	void ChangeScale(const float x, const float y);

	Vector2 GetPosition();
	float GetRotation();
	Vector2 GetScale();

	Vector2 position;
	Vector2 scale;
	float rotation;
};