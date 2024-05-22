#pragma once
#include <SDL2/SDL.h>
#include "Shapes.h"

const int TILE_SIZE = 22;  // �w�q TILE_SIZE
const int GRID_WIDTH = 10;  // �w�q GRID_WIDTH
const int GRID_HEIGHT = 20;  // �w�q GRID_HEIGHT

void draw(SDL_Renderer* renderer, const shape& s);
void render(SDL_Renderer* renderer, const shape& cur, const shape& next, int score);
void drawGrid(SDL_Renderer* renderer);
