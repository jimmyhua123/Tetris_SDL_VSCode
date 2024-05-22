#include <SDL2/SDL.h>
#include <iostream>
#include "Block.h"

// 簡單的函數來繪製方塊到 SDL 渲染器
void drawBlock(SDL_Renderer* renderer, Block& block) {
    SDL_Rect rect;
    rect.w = 20; // 方塊的寬度
    rect.h = 20; // 方塊的高度
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (block.shape[i][j] == 'x') {
                rect.x = (block.x + j) * 20; // 設定 X 位置
                rect.y = (block.y + i) * 20; // 設定 Y 位置
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // 方塊的顏色
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // 創建窗口
    window = SDL_CreateWindow("Tetris Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // 創建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    Block block(0); // 創建一個方塊
    block.x = 5; // 設置方塊在視窗中央左右，注意這裡的數值可能需要調整以適應你的窗口大小
    block.y = 0; // 設置方塊在視窗頂部

    // 主遊戲循環
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_SPACE:
                        block.rotate(); // 按空白鍵旋轉方塊
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // 設定背景顏色
        SDL_RenderClear(renderer);

        drawBlock(renderer, block); // 繪製方塊

        SDL_RenderPresent(renderer);
    }

    // 清理
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
