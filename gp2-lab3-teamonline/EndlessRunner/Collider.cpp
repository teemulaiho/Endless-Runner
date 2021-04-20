// Code From: Jerry Jonsson / Uppsala University / Game Programming 1: Computer Games in 2D | 5.0 hp | 5SD804

#include "Collider.h"

Rectangle::Rectangle() : x(0), y(0), w(0), h(0)
{
}

Rectangle::Rectangle(const Rectangle& rhs) : x(rhs.x), y(rhs.y), w(rhs.w), h(rhs.h)
{
}

Rectangle::Rectangle(int x, int y, int w, int h) : x(x), y(y), w(w), h(h)
{
}

void Rectangle::SetBounds(int x, int y, int w, int h)
{
	this->x = x; this->y = y; this->w = w; this->h = h;
}

Collider::Collider() : rect(0, 0, 0, 0)
{
}

Collider::Collider(const Collider& rhs) : rect(rhs.rect)
{
}

Collider::Collider(int x, int y, int w, int h) : rect(x, y, w, h)
{
}

void Collider::SetPosition(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void Collider::SetSize(int w, int h)
{
	rect.w = w;
	rect.h = h;
}
