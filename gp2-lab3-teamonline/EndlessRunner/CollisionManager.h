#pragma once

struct Collider;

struct CollisionManager
{
	static bool CheckCollisionAABB(Collider& lhs, Collider& rhs)
	{
		if (lhs.rect.x + lhs.rect.w < rhs.rect.x ||		// If player (rhs) is on the right side of the object (lhs).
			lhs.rect.x > rhs.rect.x + rhs.rect.w ||		// If player (rhs) is on the left side of the object (lhs).
			lhs.rect.y + lhs.rect.h < rhs.rect.y ||		// If player (rhs) is below the object (lhs).
			lhs.rect.y > rhs.rect.y + rhs.rect.h)		// If player (rhs) is above the object (lhs).
		{
			return false;
		}
		return true;
	}
};