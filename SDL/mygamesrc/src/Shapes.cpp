// c\SDL\mygamesrc\src\Shapes.cpp
#include "Shapes.h"
#include <vector>

// 定義方塊顏色
Color colors[7] = {
    {0, 255, 255},  // I
    {255, 165, 0},  // L
    {0, 0, 255},    // J
    {255, 255, 0},  // O
    {0, 255, 0},    // S
    {255, 0, 0},    // Z
    {128, 0, 128}   // T
};

// 定義方塊形狀
std::vector<std::vector<std::vector<int>>> tetrominoes = {
    {{1, 1, 1, 1}},  // I
    {{1, 0, 0}, {1, 1, 1}},  // L
    {{0, 0, 1}, {1, 1, 1}},  // J
    {{1, 1}, {1, 1}},  // O
    {{0, 1, 1}, {1, 1, 0}},  // S
    {{1, 1, 0}, {0, 1, 1}},  // Z
    {{0, 1, 0}, {1, 1, 1}}   // T
};
