#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Constants.h"
#include "Color.h"
#include "Shapes.h"
#include "Tetromino.h"
#include "GameLogic.h"
#include <string>

// 全局變量
bool running = true;
SDL_Renderer* renderer;
SDL_Window* window;
Tetromino currentPiece(0, 0, {{0}});
Tetromino nextPiece(0, 0, {{0}});
Tetromino holdPiece(0, 0, {{0}}); // 添加 holdPiece
bool holdUsed = false; // 添加 holdUsed 標誌
std::vector<std::vector<Color>> grid;
int score = 0; // 初始化分數為0
int difficulty = 0; // 初始化難度為0
int fallTime = 0;
int fallSpeed = FALL_SPEED;

// 初始化 SDL
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL 初始化失敗! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("212410012", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "視窗創建失敗! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "渲染器創建失敗! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

// 處理輸入事件
void handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = false;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    currentPiece.move(-TILE_SIZE, 0);
                    if (currentPiece.collision(grid, 0, 0)) {
                        currentPiece.move(TILE_SIZE, 0);
                    }
                    break;
                case SDLK_RIGHT:
                    currentPiece.move(TILE_SIZE, 0);
                    if (currentPiece.collision(grid, 0, 0)) {
                        currentPiece.move(-TILE_SIZE, 0);
                    }
                    break;
                case SDLK_DOWN:
                    currentPiece.move(0, TILE_SIZE);
                    if (currentPiece.collision(grid, 0, 0)) {
                        currentPiece.move(0, -TILE_SIZE);
                    }
                    break;
                case SDLK_UP:
                    currentPiece.rotate();
                    if (currentPiece.collision(grid, 0, 0)) {
                        currentPiece.rotate();
                        currentPiece.rotate();
                        currentPiece.rotate();
                    }
                    break;
                case SDLK_LSHIFT: // 使用左 Shift 鍵進行 hold 操作
                    if (!holdUsed) {
                        if (holdPiece.shape[0][0] == 0) {
                            holdPiece = currentPiece.copy();
                            holdPiece.x = 50;  // 確保 hold 方塊的位置固定
                            holdPiece.y = 50;
                            currentPiece = nextPiece;
                            nextPiece = getNewPiece();
                        } else {
                            std::swap(currentPiece, holdPiece);
                            currentPiece.x = 300;  // 確保 currentPiece 重新放回格線區
                            currentPiece.y = 0;
                        }
                        holdUsed = true;
                    }
                    break;
            }
        }
    }
}

// 更新遊戲邏輯
void update() {
    fallTime += 1;
    if (fallTime >= fallSpeed / 10) {
        currentPiece.move(0, TILE_SIZE);
        if (currentPiece.collision(grid, 0, 0)) {
            currentPiece.move(0, -TILE_SIZE);
            addToGrid(currentPiece, grid);
            currentPiece = nextPiece;
            nextPiece = getNewPiece();
            int cleared = clearRows(grid);
            score += cleared;  // 每消一行更新一次分數
            if (score >= (difficulty + 1) * 5) {
                difficulty += 1;  // 每消5行增加1難度
                fallSpeed = FALL_SPEED / (difficulty + 1);  // 更新下落速度
            }
            if (currentPiece.collision(grid, 0, 0)) {
                running = false; // 遊戲結束
            }
            holdUsed = false; // 重置 hold 標誌
        }
        fallTime = 0;
    }
}

// 渲染遊戲畫面
void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 繪製遊戲區域
    drawGrid(renderer, 300, 0);
    currentPiece.draw(renderer, 300, 0);
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (grid[y][x].r != 0 || grid[y][x].g != 0 || grid[y][x].b != 0) {
                SDL_Rect rect = {300 + x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_SetRenderDrawColor(renderer, grid[y][x].r, grid[y][x].g, grid[y][x].b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }

    // 繪製下一個方塊
    drawNextPiece(renderer, nextPiece);

    // 繪製 hold 方塊
    if (holdPiece.shape[0][0] != 0) {
        holdPiece.draw(renderer, 50, 50); // 假設 hold 方塊位置在左側固定範圍內
    }

    // 繪製分數（使用方塊來顯示）
    int scoreBaseX = 50; // 與 hold 方塊底部對齊
    int scoreBaseY = 150; // 固定在 hold 方塊下方
    int boxSize = 20; // 方塊大小
    int boxesPerRow = 10; // 每行的方塊數

    // 顯示白色方塊根據分數
    for (int i = 0; i < score; ++i) {
        int x = scoreBaseX + (i % boxesPerRow) * (boxSize + 5);
        int y = scoreBaseY + (i / boxesPerRow) * (boxSize + 5);
        SDL_Rect whiteBox = {x, y, boxSize, boxSize};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色
        SDL_RenderFillRect(renderer, &whiteBox);
    }

    // 繪製難度（使用藍色方塊來顯示）
    int difficultyBaseX = 650; // 與 next 方塊顯示的位置相同
    int difficultyBaseY = 150; // 與 score 方塊對齊

    // 顯示藍色方塊根據難度
    for (int i = 0; i < difficulty; ++i) {
        int x = difficultyBaseX + (i % boxesPerRow) * (boxSize + 5);
        int y = difficultyBaseY + (i / boxesPerRow) * (boxSize + 5);
        SDL_Rect blueBox = {x, y, boxSize, boxSize};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // 藍色
        SDL_RenderFillRect(renderer, &blueBox);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));
    if (!init()) {
        std::cerr << "初始化失敗!" << std::endl;
        return -1;
    }

    grid = createGrid();
    currentPiece = getNewPiece();
    nextPiece = getNewPiece();

    while (running) {
        handleInput();
        update();
        render();
        SDL_Delay(16); // 控制遊戲速度
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
