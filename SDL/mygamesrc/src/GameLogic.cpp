#include "GameLogic.h"
#include "EventHandler.h"

shape blocks[7] = {
    // L BLOCK
    {{255,165,0}, {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}, 5, 4, 3},
    // Z BLOCK
    {{255,0,0}, {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}, 5, 4, 3},
    // I BLOCK
    {{224,255,255}, {{1,1,1,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}}, 5, 4, 4},
    // J BLOCK
    {{0,0,255}, {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}, 5, 4, 3},
    // O BLOCK
    {{255,255,0}, {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, 5, 4, 2},
    // S BLOCK
    {{0,0,255}, {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, 5, 4, 3},
    // T BLOCK
    {{128,0,128}, {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}, 5, 4, 3}
};

void update() {
    if (left) cur.x--;
    if (right) cur.x++;
    if (down) cur.y++;
    if (up) rotate();

    // 限制方塊在範圍內移動
    if (cur.x < 0) cur.x = 0;
    if (cur.x + cur.size > 10) cur.x = 10 - cur.size;
    if (cur.y + cur.size > 20) cur.y = 20 - cur.size;
}

void rotate() {
    cur = reverseCols(transpose(cur));
}

shape reverseCols(shape s) {
    shape tmp = s;
    for(int i = 0; i < s.size; i++) {
        for(int j = 0; j < s.size / 2; j++) {
            bool t = s.matrix[i][j];
            tmp.matrix[i][j] = s.matrix[i][s.size - j - 1];
            tmp.matrix[i][s.size - j - 1] = t;
        }
    }
    return tmp;
}

shape transpose(shape s) {
    shape tmp = s;
    for(int i = 0; i < s.size; i++) {
        for(int j = 0; j < s.size; j++) {
            tmp.matrix[i][j] = s.matrix[j][i];
        }
    }
    return tmp;
}
