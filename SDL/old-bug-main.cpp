#include <SDL2/SDL.h>
#include <iostream>
#include "Block.h"

// ²�檺��ƨ�ø�s����� SDL ��V��
void drawBlock(SDL_Renderer* renderer, Block& block) {
    SDL_Rect rect;
    rect.w = 20; // ������e��
    rect.h = 20; // ���������
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (block.shape[i][j] == 'x') {
                rect.x = (block.x + j) * 20; // �]�w X ��m
                rect.y = (block.y + i) * 20; // �]�w Y ��m
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // ������C��
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // ��l�� SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // �Ыص��f
    window = SDL_CreateWindow("Tetris Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // �Ыش�V��
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    Block block(0); // �Ыؤ@�Ӥ��
    block.x = 5; // �]�m����b�����������k�A�`�N�o�̪��ƭȥi��ݭn�վ�H�A���A�����f�j�p
    block.y = 0; // �]�m����b��������

    // �D�C���`��
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
                        block.rotate(); // ���ť��������
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // �]�w�I���C��
        SDL_RenderClear(renderer);

        drawBlock(renderer, block); // ø�s���

        SDL_RenderPresent(renderer);
    }

    // �M�z
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
