#include "GameLogic.h"
#include "Tetromino.h"
#include <vector>
#include <algorithm>

std::vector<std::vector<Color>> createGrid() {
    std::vector<std::vector<Color>> grid(GRID_HEIGHT, std::vector<Color>(GRID_WIDTH, {0, 0, 0}));
    return grid;
}

void addToGrid(Tetromino& shape, std::vector<std::vector<Color>>& grid) {
    for (int i = 0; i < shape.shape->size(); ++i) {
        for (int j = 0; j < (*shape.shape)[i].size(); ++j) {
            if ((*shape.shape)[i][j]) {
                grid[(shape.y / TILE_SIZE) + i][(shape.x / TILE_SIZE) + j] = shape.color;
            }
        }
    }
}

int clearRows(std::vector<std::vector<Color>>& grid) {
    int cleared = 0;
    for (int y = GRID_HEIGHT - 1; y >= 0; --y) {
        bool full = true;
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (grid[y][x].r == 0 && grid[y][x].g == 0 && grid[y][x].b == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            cleared++;
            for (int yy = y; yy > 0; --yy) {
                grid[yy] = grid[yy - 1];
            }
            grid[0] = std::vector<Color>(GRID_WIDTH, {0, 0, 0});
            y++;
        }
    }
    return cleared;
}

Tetromino getNewPiece() {
    return Tetromino(5 * TILE_SIZE, 0, tetrominoes[rand() % tetrominoes.size()]);
}

void drawGrid(SDL_Renderer* renderer, int offsetX, int offsetY) {
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            SDL_Rect rect = {offsetX + x * TILE_SIZE, offsetY + y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}

void drawNextPiece(SDL_Renderer* renderer, Tetromino& next_piece) {
    for (int i = 0; i < next_piece.shape->size(); ++i) {
        for (int j = 0; j < (*next_piece.shape)[i].size(); ++j) {
            if ((*next_piece.shape)[i][j]) {
                SDL_Rect rect = {650 + j * 20, 40 + i * 20, 20, 20};
                SDL_SetRenderDrawColor(renderer, next_piece.color.r, next_piece.color.g, next_piece.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}
