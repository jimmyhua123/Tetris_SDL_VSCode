#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include "Constants.h"
#include "Color.h"
#include "Shapes.h"
#include "Tetromino.h"
#include "GameLogic.h"
#include "Console.h"
#include <string>
#include <map>
#include <algorithm>

using namespace std;

// 全局變量
bool running = true;
bool gameOver = false;
bool gameStarted = false;
SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;
Tetromino currentPiece(0, 0, {{0}});
Tetromino nextPiece(0, 0, {{0}});
std::vector<std::vector<Color>> grid;
int score = 0; // 初始化分數為0
int difficulty = 0; // 初始化難度為0
int fallTime = 0;
int fallSpeed = FALL_SPEED;
std::string playerID; // 玩家ID
std::map<std::string, int> playerScores; // 玩家分數表

// 按鈕結構體
struct Button {
    SDL_Rect rect;
    std::string label;
    bool pressed;
};

// endGame 函數聲明
void endGame();

// 處理輸入事件
void handleInput(Button& button) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = false;
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (x >= button.rect.x && x <= button.rect.x + button.rect.w &&
                y >= button.rect.y && y <= button.rect.y + button.rect.h) {
                button.pressed = true;
            }
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            if (button.pressed) {
                button.pressed = false;
                if (gameOver) {
                    // 重置遊戲狀態
                    grid = createGrid();
                    currentPiece = getNewPiece();
                    nextPiece = getNewPiece();
                    score = 0;
                    difficulty = 0;
                    fallSpeed = FALL_SPEED;
                    gameOver = false;
                }
            }
        } else if (e.type == SDL_KEYDOWN && !gameOver) {
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
            }
        }
    }
}

// 更新遊戲邏輯
void update() {
    if (gameOver || !gameStarted) {
        return;
    }

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
            playerScores[playerID] = score; // 更新玩家分數
            if (score >= (difficulty + 1) * 5) {
                difficulty += 1;  // 每消5行增加1難度
                fallSpeed = FALL_SPEED / (difficulty + 1);  // 更新下落速度
            }
            if (currentPiece.collision(grid, 0, 0)) {
                endGame(); // 呼叫 endGame 函數
            }
        }
        fallTime = 0;
    }
}

// 渲染遊戲畫面
void render(Button& button) {
    if (!gameStarted) {
        return;
    }

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

    // 繪製分數（使用方塊來顯示）
    int scoreBaseX = 50; // 固定位置
    int scoreBaseY = 150; // 固定位置
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

    // 繪製按鈕
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 紅色
    SDL_RenderFillRect(renderer, &button.rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色
    SDL_RenderDrawRect(renderer, &button.rect);

    SDL_RenderPresent(renderer);
}

// 定義 endGame 函數
void endGame() {
    std::cout << "Game Over!" << std::endl;
    gameOver = true; // 設置遊戲結束標誌
    running = false; // 停止遊戲循環

    // 清理 SDL 資源
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    int nCmdShow = SW_SHOW;

    // 加載玩家數據
    loadPlayers();

    // 創建控制台窗口
    createConsole();

    grid = createGrid();
    currentPiece = getNewPiece();
    nextPiece = getNewPiece();

    // 定義按鈕
    Button button;
    button.rect = {50, 50, 100, 50}; // 定義按鈕的位置和大小，放在左上角
    button.label = "Restart";
    button.pressed = false;

    while (running) {
        handleInput(button);
        update();
        render(button);

        if (_kbhit()) {
            handleConsoleInput();
        }

        SDL_Delay(16); // 控制遊戲速度
    }

    // 保存玩家數據
    savePlayers();

    return 0;
}
