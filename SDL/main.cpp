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
#include <string>  // �K�[�o�@��

// �����ܶq
bool running = true;
SDL_Renderer* renderer;
SDL_Window* window;
Tetromino currentPiece(0, 0, {{0}});
Tetromino nextPiece(0, 0, {{0}});
std::vector<std::vector<Color>> grid;
int score = 15; // ��l�Ƥ��Ƭ�0
int difficulty = 15; // ��l�����׬�0
int fallTime = 0;
int fallSpeed = FALL_SPEED;

// ��l�� SDL
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL ��l�ƥ���! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("212410012", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "�����Ыإ���! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "��V���Ыإ���! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

// �B�z��J�ƥ�
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
            }
        }
    }
}

// ��s�C���޿�
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
            score += cleared;  // �C���@���s�@������
            if (score >= (difficulty + 1) * 5) {
                difficulty += 1;  // �C��5��W�[1����
                fallSpeed = FALL_SPEED / (difficulty + 1);  // ��s�U���t��
            }
            if (currentPiece.collision(grid, 0, 0)) {
                running = false; // �C������
            }
        }
        fallTime = 0;
    }
}

// ��V�C���e��
void render() {
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
    int baseX = 10; // ���W����X��m
    int baseY = 10; // ���W����Y��m
    int boxSize = 20; // ����j�p
    int boxesPerRow = 10; // �C�檺�����

    // ��ܥզ����ھڤ���
    for (int i = 0; i < score; ++i) {
        int x = baseX + (i % boxesPerRow) * (boxSize + 5);
        int y = baseY + (i / boxesPerRow) * (boxSize + 5);
        SDL_Rect whiteBox = {x, y, boxSize, boxSize};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // �զ�
        SDL_RenderFillRect(renderer, &whiteBox);
    }

    // ø�s���ס]�ϥ��Ŧ�������ܡ^
    int difficultyBaseX = 650; // �P next �����ܪ���m�ۦP
    int difficultyBaseY = 200; // next ������U����m

    // ����Ŧ����ھ�����
    for (int i = 0; i < difficulty; ++i) {
        int x = difficultyBaseX + (i % boxesPerRow) * (boxSize + 5);
        int y = difficultyBaseY + (i / boxesPerRow) * (boxSize + 5);
        SDL_Rect blueBox = {x, y, boxSize, boxSize};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // �Ŧ�
        SDL_RenderFillRect(renderer, &blueBox);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));
    if (!init()) {
        std::cerr << "��l�ƥ���!" << std::endl;
        return -1;
    }

    grid = createGrid();
    currentPiece = getNewPiece();
    nextPiece = getNewPiece();

    while (running) {
        handleInput();
        update();
        render();
        SDL_Delay(16); // ����C���t��
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
