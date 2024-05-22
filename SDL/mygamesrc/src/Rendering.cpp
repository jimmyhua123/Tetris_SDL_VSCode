#include "Rendering.h"
#include "Constants.h"
#include "Shapes.h"
#include <SDL2/SDL.h>

// 計算偏移量
const int OFFSET_X = (1000 - TILE_SIZE * GRID_WIDTH) / 2;
const int OFFSET_Y = (600 - TILE_SIZE * GRID_HEIGHT) / 2;

void draw(SDL_Renderer* renderer, const shape& s, int offsetX, int offsetY) {
    SDL_Rect rect;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;
    for (int i = 0; i < s.size; ++i) {
        for (int j = 0; j < s.size; ++j) {
            if (s.matrix[i][j]) {
                rect.x = offsetX + (s.x + j) * TILE_SIZE;
                rect.y = offsetY + (s.y + i) * TILE_SIZE;
                SDL_SetRenderDrawColor(renderer, s.color.r, s.color.g, s.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 219, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

void drawGrid(SDL_Renderer* renderer, int offsetX, int offsetY) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    for (int i = 0; i <= GRID_WIDTH; ++i) {
        SDL_RenderDrawLine(renderer, offsetX + i * TILE_SIZE, offsetY, offsetX + i * TILE_SIZE, offsetY + GRID_HEIGHT * TILE_SIZE);
    }
    for (int i = 0; i <= GRID_HEIGHT; ++i) {
        SDL_RenderDrawLine(renderer, offsetX, offsetY + i * TILE_SIZE, offsetX + GRID_WIDTH * TILE_SIZE, offsetY + i * TILE_SIZE);
    }
}

void render(SDL_Renderer* renderer, const shape& cur, const shape& next, int score) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawGrid(renderer, OFFSET_X, OFFSET_Y);
    draw(renderer, cur, OFFSET_X, OFFSET_Y);
    
    // 在右側顯示 next 方塊
    int nextOffsetX = OFFSET_X + TILE_SIZE * (GRID_WIDTH + 2);
    int nextOffsetY = OFFSET_Y + TILE_SIZE * 2;
    draw(renderer, next, nextOffsetX, nextOffsetY);
    
    // 渲染分數
    // 這裡可以使用 SDL_ttf 庫來渲染文本
    
    SDL_RenderPresent(renderer);
}
