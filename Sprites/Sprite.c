#include "Sprite.h"


/**
 *  Cambia el sprite de la bala.
 */
void changeSpriteBullets(struct LinkedList *bullets, const int *time) {
    int timeaux = *time;

    if (timeaux % 1 == 0) {
        for (int i = 0; i < bullets->size; ++i) {
            struct Bullet *tmp = *(struct Bullet **) get(bullets, i);
            tmp->currentSprite = ++tmp->currentSprite % 4;
        }
    }
}

/**
 * Cambia el sprtie de la nave.
 */
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