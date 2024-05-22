// GameLogic.h
#pragma once
#include "Shapes.h"

extern shape blocks[7];
extern shape cur;

void update();
void rotate();
shape reverseCols(shape s);
shape transpose(shape s);


