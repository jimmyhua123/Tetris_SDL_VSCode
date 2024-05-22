#include "Rendering.h"

void draw(SDL_Renderer* renderer, const shape& s) {
    SDL_Rect rect;
    rect.w = rect.h = 22;  // °²³] TILE_SIZE ¬° 22
    for (int i = 0; i < s.size; i++) {
        for (int j = 0; j < s.size; j++) {
            if (s.matrix[i][j]) {
                rect.x = (s.x + j) * 22;
                rect.y = (s.y + i) * 22;
                SDL_SetRenderDrawColor(renderer, s.color.r, s.color.g, s.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 219, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

void render(SDL_Renderer* renderer, const shape& cur) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    draw(renderer, cur);
    SDL_RenderPresent(renderer);
}
