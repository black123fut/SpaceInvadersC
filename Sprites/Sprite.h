#ifndef SPACEINVADERS_SPRITE_H
#define SPACEINVADERS_SPRITE_H

#include "SDL2/SDL_image.h"

typedef struct _Player {
    int x;
    int y;
    int width;
    int height;
    int currentSprite;
    int time;

    SDL_Texture *sheet;
} Player;

typedef struct _Bullet {
    int y;
    int width;
    int height;
    SDL_Texture *img;
} Bullet;

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

#endif //SPACEINVADERS_SPRITE_H
