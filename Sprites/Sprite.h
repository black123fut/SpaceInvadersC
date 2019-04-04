#ifndef SPACEINVADERS_SPRITE_H
#define SPACEINVADERS_SPRITE_H

#include "SDL2/SDL_image.h"
#include "../DataStructures/LinkedList.h"

typedef struct _Player {
    int x;
    int y;
    int width;
    int height;
    int currentSprite;
    int time;
    int cooldown;

    SDL_Texture *sheet;
} Player;

struct Bullet {
    int x;
    int y;
    int width;
    int height;
    int currentSprite;
    int direction;
    SDL_Texture *sheet;
};

struct Alien {
    int x;
    int y;
    int width;
    int height;
    int row;
    int index;
    int currentSprite;
    SDL_Texture *sheet;
} ;

void changeSpriteShip(Player *pl, int max, int dir);

void changeSpriteAliens(struct LinkedList *aliens, int *time);

void changeSpriteBullets(struct LinkedList *bullets, const int *time);

#endif //SPACEINVADERS_SPRITE_H
