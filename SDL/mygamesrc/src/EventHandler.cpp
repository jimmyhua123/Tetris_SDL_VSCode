#include "EventHandler.h"
#include <SDL2/SDL.h>
#include "GameLogic.h"  // 確保 cur 變數被聲明

void input() {
    up = down = left = right = 0;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    left = 1;
                    break;
                case SDLK_RIGHT:
                    right = 1;
                    break;
                case SDLK_UP:
                    up = 1;
                    break;
                case SDLK_DOWN:
                    down = 1;
                    break;
                case SDLK_ESCAPE:
                    running = false;
                    break;
            }
        }
    }
}
