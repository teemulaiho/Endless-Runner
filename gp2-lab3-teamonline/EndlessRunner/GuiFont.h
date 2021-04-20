#pragma once

#include "AssetManager.h"
#include <string>
#include "SDL.h"

struct GuiFont {
	Font*								font;
	SDL_Color							textColor;
	std::string							text;
};