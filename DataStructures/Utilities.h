#ifndef SPACEINVADERS_UTILITIES_H
#define SPACEINVADERS_UTILITIES_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdbool.h>
#include "LinkedList.h"
#include "../Sprites/Sprite.h"

bool checkCollision(struct Bullet *bullet, struct Alien* alien);

SDL_Surface *loadImage(char* path);

SDL_Renderer *init();

void getAliens(SDL_Renderer *renderer, struct LinkedList *aliens);

SDL_Surface *getAlienImage(int row);

SDL_Surface *getBulletImage(int dir);

void setAttributes(struct Alien *alien, int column, int index, int row);

void addBulletPlayer(struct LinkedList *bullets, Player *pl, SDL_Renderer * renderer);

void addBulletAlien(struct LinkedList *bullets, struct Alien *alien, SDL_Renderer * renderer);

void free_alien(void *data);

void free_bullet(void *data);

#endif //SPACEINVADERS_UTILITIES_H
