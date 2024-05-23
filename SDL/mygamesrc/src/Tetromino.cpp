#include "Tetromino.h"
#include <algorithm>

Tetromino::Tetromino(int startX, int startY, std::vector<std::vector<int>> shape) :
    x(startX), y(startY), shape(shape), rotation(0) {
    color = colors[std::distance(tetrominoes.begin(), 
        std::find(tetrominoes.begin(), tetrominoes.end(), shape))];
}

void Tetromino::draw(SDL_Renderer* renderer, int offsetX, int offsetY) {
    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            if (shape[i][j]) {
                SDL_Rect rect = {offsetX + x + j * TILE_SIZE, offsetY + y + i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                // 填充方塊
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                // 繪製方塊邊框
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

void Tetromino::rotate() {
    std::vector<std::vector<int>> rotated(shape[0].size(), std::vector<int>(shape.size()));
    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            rotated[j][shape.size() - 1 - i] = shape[i][j];
        }
    }
    shape = rotated;
}

void Tetromino::move(int dx, int dy) {
    x += dx;
    y += dy;
}

bool Tetromino::collision(const std::vector<std::vector<Color>>& grid, int offsetX, int offsetY) {
    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            if (shape[i][j]) {
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
