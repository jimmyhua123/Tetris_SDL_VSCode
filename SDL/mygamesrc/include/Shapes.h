// Shapes.h
#pragma once
#include <SDL2/SDL.h>

struct block {
    SDL_Color color;
    bool active;
};

struct shape {
    SDL_Color color;
    bool matrix[4][4];
    double x, y;
    int size;
};
