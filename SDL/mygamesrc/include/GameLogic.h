#pragma once
#include <vector>
#include "Color.h"
#include "Tetromino.h"
#include "Constants.h"
#include <SDL2/SDL.h>

std::vector<std::vector<Color>> createGrid();
void addToGrid(Tetromino& shape, std::vector<std::vector<Color>>& grid);
int clearRows(std::vector<std::vector<Color>>& grid);
Tetromino getNewPiece();
void drawGrid(SDL_Renderer* renderer, int offsetX, int offsetY);
void drawNextPiece(SDL_Renderer* renderer, Tetromino& next_piece);
void drawScore(SDL_Renderer* renderer, int score);  // 去掉字型相關代碼
