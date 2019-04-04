#include "Sprite.h"

void changeSpriteAliens(struct LinkedList *aliens, int *time) {
    *time = *time + 1;
    int timeaux = *time;
    if (timeaux > 50) {
        *time = 0;
    }
    if (timeaux % 10 == 0) {
        for (int i = 0; i < aliens->size; ++i) {
            struct Alien *tmp = *(struct Alien **) get(aliens, i);
            tmp->currentSprite = ++tmp->currentSprite % 2;
        }
    }
}

void changeSpriteBullets(struct LinkedList *bullets, const int *time) {
    int timeaux = *time;

    if (timeaux % 1 == 0) {
        for (int i = 0; i < bullets->size; ++i) {
            struct Bullet *tmp = *(struct Bullet **) get(bullets, i);
            tmp->currentSprite = ++tmp->currentSprite % 4;
        }
    }
}

void changeSpriteShip(Player *pl, int max, int dir) {
    pl->time++;
    if (pl->time > 5) {
        pl->time = 0;
    }
    if ((pl->currentSprite != max) && (pl->time % 5 == 0)) {
        pl->currentSprite += dir;
        pl->time = 0;
    }
}