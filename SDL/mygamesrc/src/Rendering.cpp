#include "Rendering.h"
#include <string>

void drawGrid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);  // �]�m��u�C��
    for (int i = 0; i <= GRID_WIDTH; ++i) {
        SDL_RenderDrawLine(renderer, i * TILE_SIZE, 0, i * TILE_SIZE, GRID_HEIGHT * TILE_SIZE);
    }
    for (int i = 0; i <= GRID_HEIGHT; ++i) {
        SDL_RenderDrawLine(renderer, 0, i * TILE_SIZE, GRID_WIDTH * TILE_SIZE, i * TILE_SIZE);
    }
}

void draw(SDL_Renderer* renderer, const shape& s) {
    SDL_Rect rect;
    rect.w = rect.h = TILE_SIZE;
    for (int i = 0; i < s.size; i++) {
        for (int j = 0; j < s.size; j++) {
            if (s.matrix[i][j]) {
                rect.x = (s.x + j) * TILE_SIZE;
                rect.y = (s.y + i) * TILE_SIZE;
                SDL_SetRenderDrawColor(renderer, s.color.r, s.color.g, s.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 219, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

void render(SDL_Renderer* renderer, const shape& cur, const shape& next, int score) {
    // �M�̨ó]�m�I���C��
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);  // �]�m�D�C���ϭI���C��
    SDL_RenderClear(renderer);

    // ø�s����i���ʽd��I��
    SDL_Rect playArea;
    playArea.x = 0;
    playArea.y = 0;
    playArea.w = GRID_WIDTH * TILE_SIZE;
    playArea.h = GRID_HEIGHT * TILE_SIZE;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // �]�m�C���ϭI���C��
    SDL_RenderFillRect(renderer, &playArea);

    // ø�s��u
    drawGrid(renderer);

    // ø�s��e���
    draw(renderer, cur);

    // ø�s�U�@�Ӥ��
    shape nextPosition = next;
    nextPosition.x = GRID_WIDTH + 2; // ��b�C���ϥk��
    nextPosition.y = 2;
    draw(renderer, nextPosition);

    // ��ܤ���
    std::string scoreText = "Score: " + std::to_string(score);
    // �o�̻ݭn�ϥ� SDL_ttf �Ψ�L�w�Ӵ�V��r�A���]�w����� renderText
    // renderText(renderer, scoreText, GRID_WIDTH + 2, 10);

    SDL_RenderPresent(renderer);
}
