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

// Ы?秖
bool running = true;
bool gameOver = false;
bool gameStarted = false;
SDL_Renderer* renderer;
SDL_Window* window;
Tetromino currentPiece(0, 0, {{0}});
Tetromino nextPiece(0, 0, {{0}});
std::vector<std::vector<Color>> grid;
int score = 0; // ﹍てだ??0
int difficulty = 0; // ﹍て??0
int fallTime = 0;
int fallSpeed = FALL_SPEED;
std::string playerID; // 產ID
std::map<std::string, int> playerScores; // 產だ?

// ??疼蔨
struct Button {
    SDL_Rect rect;
    std::string label;
    bool pressed;
};

// ﹍て SDL
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL ﹍てア?! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("212410012", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        std::cerr << "怠?ア?! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr) {
        std::cerr << "磋琕竟?ア?! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

// ?瞶?ㄆン
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
                    // 竚村???
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

// 穝村???
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
            score += cleared;  // –︽穝Ωだ?
            playerScores[playerID] = score; // 穝產だ?
            if (score >= (difficulty + 1) * 5) {
                difficulty += 1;  // –5︽糤1?
                fallSpeed = FALL_SPEED / (difficulty + 1);  // 穝辅硉
            }
            if (currentPiece.collision(grid, 0, 0)) {
                gameOver = true; // 村??
            }
        }
        fallTime = 0;
    }
}

// 磋琕村??
void render(Button& button) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // ?村??办
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

    // ??よ?
    drawNextPiece(renderer, nextPiece);

    // ?だ?ㄏノよ???ボ
    int scoreBaseX = 50; // ㏕﹚竚
    int scoreBaseY = 150; // ㏕﹚竚
    int boxSize = 20; // よ?
    int boxesPerRow = 10; // –︽よ??

    // ?ボフ︹よ?誹だ?
    for (int i = 0; i < score; ++i) {
        int x = scoreBaseX + (i % boxesPerRow) * (boxSize + 5);
        int y = scoreBaseY + (i / boxesPerRow) * (boxSize + 5);
        SDL_Rect whiteBox = {x, y, boxSize, boxSize};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // フ︹
        SDL_RenderFillRect(renderer, &whiteBox);
    }

    // ??ㄏノ?︹よ???ボ
    int difficultyBaseX = 650; // 蒓 next よ??ボ竚
    int difficultyBaseY = 150; // 蒓 score よ???

    // ?ボ?︹よ?誹?
    for (int i = 0; i < difficulty; ++i) {
        int x = difficultyBaseX + (i % boxesPerRow) * (boxSize + 5);
        int y = difficultyBaseY + (i / boxesPerRow) * (boxSize + 5);
        SDL_Rect blueBox = {x, y, boxSize, boxSize};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // ?︹
        SDL_RenderFillRect(renderer, &blueBox);
    }

    // ??
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // ?︹
    SDL_RenderFillRect(renderer, &button.rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // フ︹
    SDL_RenderDrawRect(renderer, &button.rect);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    int nCmdShow = SW_SHOW;

    // ﹍て SDL
    if (!initSDL()) {
        std::cerr << "﹍てア?!" << std::endl;
        return -1;
    }

    // ?產?誹
    loadPlayers();

    // ?北怠
    createConsole();

    grid = createGrid();
    currentPiece = getNewPiece();
    nextPiece = getNewPiece();

    // ﹚??
    Button button;
    button.rect = {50, 50, 100, 50}; // ﹚??竚㎝オà
    button.label = "Restart";
    button.pressed = false;

    while (running) {
        handleInput(button);
        update();
        render(button);

        if (_kbhit()) {
            handleConsoleInput();
        }

        SDL_Delay(16); // 北村?硉
    }

    // 玂產?誹
    savePlayers();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
