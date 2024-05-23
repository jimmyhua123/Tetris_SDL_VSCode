#pragma once
#include <vector>
#include "Color.h"
#include "Shapes.h"
#include "Constants.h"
#include <SDL2/SDL.h>

class Tetromino {
public:
    int x, y;
    std::vector<std::vector<int>> shape;
    Color color;
    int rotation;

    Tetromino(int startX, int startY, std::vector<std::vector<int>> shape);

    void draw(SDL_Renderer* renderer, int offsetX, int offsetY);
    void rotate();
    void move(int dx, int dy);
    bool collision(const std::vector<std::vector<Color>>& grid, int offsetX, int offsetY);
};
