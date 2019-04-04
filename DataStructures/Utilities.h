#ifndef SPACEINVADERS_UTILITIES_H
#define SPACEINVADERS_UTILITIES_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "LinkedList.h"
#include "../Sprites/Sprite.h"

void changeSpriteShip(Player *pl, int max, int dir);

void changeSpriteAliens(struct LinkedList *aliens, int *time);

SDL_Surface *loadImage(char* path);

SDL_Renderer *init();

void free_alien(void *data);

void getAliens(SDL_Renderer *renderer, struct LinkedList *aliens);

SDL_Surface *getAlienImage(int row);

void setAttributes(struct Alien *alien, int column, int index, int row);

#endif //SPACEINVADERS_UTILITIES_H
