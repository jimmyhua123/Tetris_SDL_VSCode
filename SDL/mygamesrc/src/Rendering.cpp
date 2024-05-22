#include "Rendering.h"
#include <string>

void drawGrid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);  // 設置格線顏色
    for (int i = 0; i <= GRID_WIDTH; ++i) {
        SDL_RenderDrawLine(renderer, i * TILE_SIZE, 0, i * TILE_SIZE, GRID_HEIGHT * TILE_SIZE);
    }
    for (int i = 0; i <= GRID_HEIGHT; ++i) {
        SDL_RenderDrawLine(renderer, 0, i * TILE_SIZE, GRID_WIDTH * TILE_SIZE, i * TILE_SIZE);
    }
}

void draw(SDL_Renderer* renderer, const shape& s) {
    SDL_Rect rect;
    rect.w = rect.h = TILE_SIZE;
    for (int i = 0; i < s.size; i++) {
        for (int j = 0; j < s.size; j++) {
            if (s.matrix[i][j]) {
                rect.x = (s.x + j) * TILE_SIZE;
                rect.y = (s.y + i) * TILE_SIZE;
                SDL_SetRenderDrawColor(renderer, s.color.r, s.color.g, s.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 219, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

void render(SDL_Renderer* renderer, const shape& cur, const shape& next, int score) {
    // 清屏並設置背景顏色
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);  // 設置非遊戲區背景顏色
    SDL_RenderClear(renderer);

    // 繪製方塊可移動範圍背景
    SDL_Rect playArea;
    playArea.x = 0;
    playArea.y = 0;
    playArea.w = GRID_WIDTH * TILE_SIZE;
    playArea.h = GRID_HEIGHT * TILE_SIZE;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // 設置遊戲區背景顏色
    SDL_RenderFillRect(renderer, &playArea);

    // 繪製格線
    drawGrid(renderer);

    // 繪製當前方塊
    draw(renderer, cur);

    // 繪製下一個方塊
    shape nextPosition = next;
    nextPosition.x = GRID_WIDTH + 2; // 放在遊戲區右邊
    nextPosition.y = 2;
    draw(renderer, nextPosition);

    // 顯示分數
    std::string scoreText = "Score: " + std::to_string(score);
    // 這裡需要使用 SDL_ttf 或其他庫來渲染文字，假設已有函數 renderText
    // renderText(renderer, scoreText, GRID_WIDTH + 2, 10);

    SDL_RenderPresent(renderer);
}
