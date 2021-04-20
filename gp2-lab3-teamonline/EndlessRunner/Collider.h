// Code From: Jerry Jonsson / Uppsala University / Game Programming 1: Computer Games in 2D | 5.0 hp | 5SD804

#pragma once

struct Rectangle
{
	int x{};
	int y{};
	int w{};
	int h{};

	Rectangle();
	Rectangle(const Rectangle& rhs);
	Rectangle(int x, int y, int w, int h);

	void SetBounds(int x, int y, int w, int h);
};

struct Collider
{
	Collider();	
	Collider(const Collider& rhs);
	Collider(int x, int y, int w, int h);

	void SetPosition(int x, int y);
	void SetSize(int w, int h);

	Rectangle rect;
};