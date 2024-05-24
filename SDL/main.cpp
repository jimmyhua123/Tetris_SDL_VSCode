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

// �����ܶq
bool running = true;
bool gameOver = false;
bool gameStarted = false;
SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;
Tetromino currentPiece(0, 0, {{0}});
Tetromino nextPiece(0, 0, {{0}});
std::vector<std::vector<Color>> grid;
int score = 0; // ��l�Ƥ��Ƭ�0
int difficulty = 0; // ��l�����׬�0
int fallTime = 0;
int fallSpeed = FALL_SPEED;
std::string playerID; // ���aID
std::map<std::string, int> playerScores; // ���a���ƪ�

// ���s���c��
struct Button {
    SDL_Rect rect;
    std::string label;
    bool pressed;
};

// endGame ����n��
void endGame();

// �B�z��J�ƥ�
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
                    // ���m�C�����A
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

// ��s�C���޿�
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
            score += cleared;  // �C���@���s�@������
            playerScores[playerID] = score; // ��s���a����
            if (score >= (difficulty + 1) * 5) {
                difficulty += 1;  // �C��5��W�[1����
                fallSpeed = FALL_SPEED / (difficulty + 1);  // ��s�U���t��
            }
            if (currentPiece.collision(grid, 0, 0)) {
                endGame(); // �I�s endGame ���
            }
        }
        fallTime = 0;
    }
}

// ��V�C���e��
void render(Button& button) {
    if (!gameStarted) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // ø�s�C���ϰ�
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

    // ø�s�U�@�Ӥ��
    drawNextPiece(renderer, nextPiece);

    // ø�s���ơ]�ϥΤ������ܡ^
    int scoreBaseX = 50; // �T�w��m
    int scoreBaseY = 150; // �T�w��m
    int boxSize = 20; // ����j�p
    int boxesPerRow = 10; // �C�檺�����

    // ��ܥզ����ھڤ���
    for (int i = 0; i < score; ++i) {
        int x = scoreBaseX + (i % boxesPerRow) * (boxSize + 5);
        int y = scoreBaseY + (i / boxesPerRow) * (boxSize + 5);
        SDL_Rect whiteBox = {x, y, boxSize, boxSize};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // �զ�
        SDL_RenderFillRect(renderer, &whiteBox);
    }

    // ø�s���ס]�ϥ��Ŧ�������ܡ^
    int difficultyBaseX = 650; // �P next �����ܪ���m�ۦP
    int difficultyBaseY = 150; // �P score ������

    // ����Ŧ����ھ�����
    for (int i = 0; i < difficulty; ++i) {
        int x = difficultyBaseX + (i % boxesPerRow) * (boxSize + 5);
        int y = difficultyBaseY + (i / boxesPerRow) * (boxSize + 5);
        SDL_Rect blueBox = {x, y, boxSize, boxSize};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // �Ŧ�
        SDL_RenderFillRect(renderer, &blueBox);
    }

    // ø�s���s
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // ����
    SDL_RenderFillRect(renderer, &button.rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // �զ�
    SDL_RenderDrawRect(renderer, &button.rect);

    SDL_RenderPresent(renderer);
}

// �w�q endGame ���
void endGame() {
    std::cout << "Game Over!" << std::endl;
    gameOver = true; // �]�m�C�������лx
    running = false; // ����C���`��

    // �M�z SDL �귽
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

    // �[�����a�ƾ�
    loadPlayers();

    // �Ыر���x���f
    createConsole();

    grid = createGrid();
    currentPiece = getNewPiece();
    nextPiece = getNewPiece();

    // �w�q���s
    Button button;
    button.rect = {50, 50, 100, 50}; // �w�q���s����m�M�j�p�A��b���W��
    button.label = "Restart";
    button.pressed = false;

    while (running) {
        handleInput(button);
        update();
        render(button);

        if (_kbhit()) {
            handleConsoleInput();
        }

        SDL_Delay(16); // ����C���t��
    }

    // �O�s���a�ƾ�
    savePlayers();

    return 0;
}
