#pragma once
#include <vector>
#include "Color.h"
#include "Shapes.h"
#include "Constants.h"
#include <SDL2/SDL.h>

class Tetromino {
public:
    int x, y;
    int rotation;
    Color color; // �K�[�C���ݩ�
    std::vector<std::vector<int>>* shape; // �ϥΫ�?

    Tetromino(int startX, int startY, const std::vector<std::vector<int>>& shape);
    ~Tetromino();

    void draw(SDL_Renderer* renderer, int offsetX, int offsetY);
    void rotate();
    void move(int dx, int dy);
    bool collision(const std::vector<std::vector<Color>>& grid, int offsetX, int offsetY);
};
