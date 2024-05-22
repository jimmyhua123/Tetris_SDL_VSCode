#include "Rendering.h"
#include "EventHandler.h"
#include "GameLogic.h"
#include "Shapes.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <time.h>
bool running = true;
SDL_Renderer* renderer;
SDL_Window* window;
shape cur;
bool left = false, right = false, up = false, down = false;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Game Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    srand(time(NULL));  // ��l���H���ƥͦ���
    cur = blocks[rand() % 7];  // ��l�� cur ���@���H���Ϊ�

    while (running) {
        input();  // �B�z��J
        update(); // ��s�C���޿�
        render(renderer, cur);  // ��V��e�Ϊ�
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
