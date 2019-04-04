#include "Block.h"

void free_block(void *data) {
    free(*(struct Block **) data);
}

void free_rect(void *data) {
    free(*(struct SDL_Rect **) data);
}

void generateShields(struct LinkedList *shields) {
    for (int i = 0; i < 4; ++i) {
        struct Block *block = (struct Block *) malloc(sizeof(struct Block));
        createBlock(block, 100 + i * (80 + 126));
        printf("cosi: %i", i);
        add(shields, &block);
        printf("cosi2: %i", i);
    }
    struct Block *coso = (struct Block *) get(shields, 0);
    printf("\nsize shields: %i, primero: %i\n", shields->size, coso->shield->size);
}

void createBlock(struct Block *block, int xPos) {
    buildBlockMatrix(block);
    printf("Entro :)");
    block->shield = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    createList(block->shield, sizeof(struct SDL_Rect *), free_rect);

    printf("Entro :)");
    for (int i = 0; i < 14; ++i) {
        for (int j = 0; j < 16; ++j) {
            if (block->state[i][j] != 0) {
                struct SDL_Rect *rect = (struct SDL_Rect *) malloc(sizeof(struct SDL_Rect));
                rect->x = xPos + j * 5;
                rect->y = 530 + i * 5;
                rect->w = rect->h = 5;
                rect->i = i;
                rect->j = j;
                add(block->shield, &rect);
            }
        }
    }
    printf("size: %i", block->shield->size);
}

bool collision(struct SDL_Rect *rect, struct Bullet *bullet) {
    return
            (bullet->x) < (rect->x + rect->w) &&
            (bullet->y) < (rect->y + rect->h) &&
            (bullet->x + bullet->width) > (rect->x) &&
            (bullet->y + bullet->height) > (rect->y);
}

void searchCollision(struct LinkedList *bullets, struct LinkedList *shields) {
    for (int i = 0; i < 4; ++i) {
        struct Block *tmp = *(struct Block **) get(shields, i);

        for (int j = 0; j< tmp->shield->size; ++j) {
            struct SDL_Rect *rect = *(struct SDL_Rect **) get(tmp->shield, j);

            for (int k = 0; k < bullets->size; ++k) {
                struct Bullet *tmpbullet = *(struct Bullet **) get(bullets, k);

                if (collision(rect, tmpbullet)) {
                    deleteRect(tmp, rect->i, rect->j);
                    delete_node(bullets, k);
                    delete_node(tmp->shield, j);
                }

            }
        }
    }
}

void deleteRect(struct Block *block, int i, int j) {
    block->state[i][j] = 0;
}

void buildBlockMatrix(struct Block *block) {
    int tmp[14][16] = {{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
                       {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                       {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
                       {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
                       {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                       {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                       {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}};

    for (int i = 0; i < 14; ++i) {
        for (int j = 0; j < 16; ++j) {
            block->state[i][j] = tmp[i][j];
        }
        printf("matriz: %i \n", i);
    }
}