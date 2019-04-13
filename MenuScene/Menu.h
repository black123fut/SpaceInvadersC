
#ifndef SPACEINVADERS_MENU_H
#define SPACEINVADERS_MENU_H

#include <SDL_render.h>
#include "../DataStructures/Utilities.h"

#include <stdbool.h>

void drawMenu(SDL_Renderer *renderer, SDL_Texture *background, int current,
        SDL_Texture* clientPlayerText, SDL_Texture* observerText, bool playerConnected, bool serverConnected);

int menu(bool playerConnected, bool serverConnected, SDL_Renderer *renderer);

#endif //SPACEINVADERS_MENU_H
