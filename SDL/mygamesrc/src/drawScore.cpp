#include <SDL2/SDL.h>
#include <string>

void drawScore(SDL_Renderer* renderer, int score) {
    std::string scoreText = "Score: " + std::to_string(score);
    int x = 650;
    int y = 100;
    int charWidth = 20;
    int charHeight = 30;

    for (char& c : scoreText) {
        SDL_Rect rect = {x, y, charWidth, charHeight};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);

        x += charWidth + 5; // Move to the next character position
    }
}
