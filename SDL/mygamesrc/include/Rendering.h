#pragma once
#include <SDL2/SDL.h>
#include "Shapes.h"
#include "Constants.h"

void draw(SDL_Renderer* renderer, const shape& s, int offsetX, int offsetY);
void drawGrid(SDL_Renderer* renderer, int offsetX, int offsetY);
void render(SDL_Renderer* renderer, const shape& cur, const shape& next, int score);
