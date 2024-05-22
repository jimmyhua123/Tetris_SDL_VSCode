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
    
    srand(time(NULL));  // ��l���H���ƥͦ���
    cur = blocks[rand() % 7];  // ��l�� cur ���@���H���Ϊ�
    next = blocks[rand() % 7]; // ��l�� next ���@���H���Ϊ�

    while (running) {
        input();  // �B�z��J
        update(); // ��s�C���޿�
        render(renderer, cur, next, score);  // ��V��e�Ϊ��B�U�@�ӧΪ��M����
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
