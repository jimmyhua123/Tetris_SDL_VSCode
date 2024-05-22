#include "GameLogic.h"
#include "EventHandler.h"
#include "Constants.h"
#include <cstdlib>  // 包含標準庫以使用 rand 函數

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

shape cur;
shape next;

bool fixedGrid[GRID_HEIGHT][GRID_WIDTH] = {{0}};  // 初始化已固定方塊的網格

bool checkCollision(int offsetX, int offsetY, shape* s) {
    if (s == nullptr) {
        s = &cur;
    }
    for (int i = 0; i < s->size; i++) {
        for (int j = 0; j < s->size; j++) {
            if (s->matrix[i][j]) {
                int newX = s->x + j + offsetX;
                int newY = s->y + i + offsetY;

                if (newY >= GRID_HEIGHT || // 到底部
                    newX < 0 || newX >= GRID_WIDTH || // 超出左右邊界
                    (newY >= 0 && newX >= 0 && newX < GRID_WIDTH && newY < GRID_HEIGHT && fixedGrid[newY][newX])) {  // 碰到固定方塊
                    return true;
                }
            }
        }
    }
    return false;
}

void fixCurrentBlock() {
    // 將當前方塊固定在網格上
    for (int i = 0; i < cur.size; i++) {
        for (int j = 0; j < cur.size; j++) {
            if (cur.matrix[i][j]) {
                int newX = cur.x + j;
                int newY = cur.y + i;
                if (newY >= 0 && newY < GRID_HEIGHT && newX >= 0 && newX < GRID_WIDTH) {
                    fixedGrid[newY][newX] = true;
                }
            }
        }
    }
}

void update() {
    shape oldState = cur;  // 保存當前狀態以便於回滾

    if (left) cur.x--;
    if (right) cur.x++;
    if (down) cur.y++;
    if (up) rotate();

    // 檢查碰撞並修正位置
    if (checkCollision(0, 0, nullptr)) {
        cur = oldState;  // 回滾到之前的狀態
        if (down) {
            fixCurrentBlock();  // 固定當前方塊
            cur = next;  // 生成新方塊
            next = blocks[rand() % 7];  // 更新下一個方塊
            if (checkCollision(0, 0, nullptr)) {
                // 如果新生成的方塊馬上碰撞，遊戲結束邏輯
                running = false;
            }
        }
    }

    // 限制方塊在範圍內移動
    if (cur.x < 0) cur.x = 0;
    if (cur.x + cur.size > GRID_WIDTH) cur.x = GRID_WIDTH - cur.size;
    if (cur.y + cur.size > GRID_HEIGHT) cur.y = GRID_HEIGHT - cur.size;
}

void rotate() {
    shape rotated = reverseCols(transpose(cur));
    // 檢查旋轉後是否會碰撞，如果不會碰撞，則進行旋轉
    if (!checkCollision(0, 0, &rotated)) {
        cur = rotated;
    } else {
        // Wall kick 機制，嘗試不同的偏移來避免碰撞
        const int offsets[5][2] = {
            {1, 0},   // 向右移動
            {-1, 0},  // 向左移動
            {0, 1},   // 向下移動
            {1, 1},   // 右下移動
            {-1, 1}   // 左下移動
        };
        for (int i = 0; i < 5; ++i) {
            if (!checkCollision(offsets[i][0], offsets[i][1], &rotated)) {
                cur = rotated;
                cur.x += offsets[i][0];
                cur.y += offsets[i][1];
                return;
            }
        }
    }
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
