#ifndef SPACEINVADERS_SPRITE_H
#define SPACEINVADERS_SPRITE_H

#include "SDL2/SDL_image.h"

typedef struct _Player {
    int x;
    int y;
    int width;
    int heigth;

    SDL_Texture *sheet;
} Player;



#endif //SPACEINVADERS_SPRITE_H
