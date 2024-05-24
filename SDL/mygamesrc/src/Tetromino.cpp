#include "Tetromino.h"
#include <algorithm>

Tetromino::Tetromino(int startX, int startY, const std::vector<std::vector<int>>& shape) :
    x(startX), y(startY), rotation(0) {
    this->shape = new std::vector<std::vector<int>>(shape);
    int index = std::distance(tetrominoes.begin(), std::find(tetrominoes.begin(), tetrominoes.end(), shape));
    color = colors[index];
}

Tetromino::~Tetromino() {
    delete shape;
}

void Tetromino::draw(SDL_Renderer* renderer, int offsetX, int offsetY) {
    for (int i = 0; i < shape->size(); ++i) {
        for (int j = 0; j < (*shape)[i].size(); ++j) {
            if ((*shape)[i][j]) {
                SDL_Rect rect = {offsetX + x + j * TILE_SIZE, offsetY + y + i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void Tetromino::rotate() {
    std::vector<std::vector<int>> rotated(shape->at(0).size(), std::vector<int>(shape->size()));
    for (int i = 0; i < shape->size(); ++i) {
        for (int j = 0; j < shape->at(i).size(); ++j) {
            rotated[j][shape->size() - 1 - i] = shape->at(i)[j];
        }
    }
    *shape = rotated;
}

void Tetromino::move(int dx, int dy) {
    x += dx;
    y += dy;
}

bool Tetromino::collision(const std::vector<std::vector<Color>>& grid, int offsetX, int offsetY) {
    for (int i = 0; i < shape->size(); ++i) {
        for (int j = 0; j < shape->at(i).size(); ++j) {
            if (shape->at(i)[j]) {
                int newX = (x + offsetX) / TILE_SIZE + j;
                int newY = (y + offsetY) / TILE_SIZE + i;
                if (newX < 0 || newX >= GRID_WIDTH || newY < 0 || newY >= GRID_HEIGHT || 
                    grid[newY][newX].r != 0 || grid[newY][newX].g != 0 || grid[newY][newX].b != 0) {
                    return true;
                }
            }
        }
    }
    return false;
}
