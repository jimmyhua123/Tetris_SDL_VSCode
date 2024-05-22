#include "Rendering.h"
#include "EventHandler.h"
#include "GameLogic.h"
#include "Shapes.h"
#include "Constants.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <time.h>

bool running = true;
SDL_Renderer* renderer;
SDL_Window* window;
int score = 0;
bool left = false, right = false, up = false, down = false;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("212410012", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    srand(time(NULL));  // 初始化隨機數生成器
    cur = blocks[rand() % 7];  // 初始化 cur 為一個隨機形狀
    next = blocks[rand() % 7]; // 初始化 next 為一個隨機形狀

    while (running) {
        input();  // 處理輸入
        update(); // 更新遊戲邏輯
        render(renderer, cur, next, score);  // 渲染當前形狀、下一個形狀和分數
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
