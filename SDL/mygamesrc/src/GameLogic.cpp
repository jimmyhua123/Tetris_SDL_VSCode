#include "GameLogic.h"
#include "EventHandler.h"
#include "Constants.h"
#include <cstdlib>  // �]�t�зǮw�H�ϥ� rand ���

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

bool fixedGrid[GRID_HEIGHT][GRID_WIDTH] = {{0}};  // ��l�Ƥw�T�w���������

bool checkCollision(int offsetX, int offsetY, shape* s) {
    if (s == nullptr) {
        s = &cur;
    }
    for (int i = 0; i < s->size; i++) {
        for (int j = 0; j < s->size; j++) {
            if (s->matrix[i][j]) {
                int newX = s->x + j + offsetX;
                int newY = s->y + i + offsetY;

                if (newY >= GRID_HEIGHT || // �쩳��
                    newX < 0 || newX >= GRID_WIDTH || // �W�X���k���
                    (newY >= 0 && newX >= 0 && newX < GRID_WIDTH && newY < GRID_HEIGHT && fixedGrid[newY][newX])) {  // �I��T�w���
                    return true;
                }
            }
        }
    }
    return false;
}

void fixCurrentBlock() {
    // �N��e����T�w�b����W
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
    shape oldState = cur;  // �O�s��e���A�H�K��^�u

    if (left) cur.x--;
    if (right) cur.x++;
    if (down) cur.y++;
    if (up) rotate();

    // �ˬd�I���íץ���m
    if (checkCollision(0, 0, nullptr)) {
        cur = oldState;  // �^�u�줧�e�����A
        if (down) {
            fixCurrentBlock();  // �T�w��e���
            cur = next;  // �ͦ��s���
            next = blocks[rand() % 7];  // ��s�U�@�Ӥ��
            if (checkCollision(0, 0, nullptr)) {
                // �p�G�s�ͦ���������W�I���A�C�������޿�
                running = false;
            }
        }
    }

    // �������b�d�򤺲���
    if (cur.x < 0) cur.x = 0;
    if (cur.x + cur.size > GRID_WIDTH) cur.x = GRID_WIDTH - cur.size;
    if (cur.y + cur.size > GRID_HEIGHT) cur.y = GRID_HEIGHT - cur.size;
}

void rotate() {
    shape rotated = reverseCols(transpose(cur));
    // �ˬd�����O�_�|�I���A�p�G���|�I���A�h�i�����
    if (!checkCollision(0, 0, &rotated)) {
        cur = rotated;
    } else {
        // Wall kick ����A���դ��P���������קK�I��
        const int offsets[5][2] = {
            {1, 0},   // �V�k����
            {-1, 0},  // �V������
            {0, 1},   // �V�U����
            {1, 1},   // �k�U����
            {-1, 1}   // ���U����
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
