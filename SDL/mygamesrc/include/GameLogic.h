#pragma once
#include "Shapes.h"
#include "Constants.h"

extern shape blocks[7];
extern shape cur;
extern shape next;
extern bool fixedGrid[GRID_HEIGHT][GRID_WIDTH];

void update();
void rotate();
shape reverseCols(shape s);
shape transpose(shape s);
bool checkCollision(int offsetX = 0, int offsetY = 0, shape* s = nullptr);
