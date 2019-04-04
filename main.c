#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "DataStructures/LinkedList.h"
#include "Sprites/Sprite.h"
#include "DataStructures/Utilities.h"

int timeAlien = 0, border = 10, speed = 1;

void draw(SDL_Renderer *renderer, Player *pl, struct LinkedList *aliens) {

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //El jugador
    SDL_Rect srcRect = { pl->width * pl->currentSprite, 0, pl->width, pl->height};
    SDL_Rect rect = { pl->x, pl->y, pl->width, pl->height };
    SDL_RenderCopyEx(renderer, pl->sheet, &srcRect, &rect, 0, NULL, 0);

    changeSpriteAliens(aliens, &timeAlien);

    for (int i = 0; i < aliens->size; ++i) {
        struct Alien *tmp = *(struct Alien **) get(aliens, i);
        SDL_Rect alRect = { tmp->width * tmp->currentSprite, 0, tmp->width, tmp->height};
        SDL_Rect al_rec = { tmp->x, tmp->y, tmp->width, tmp->height };
        SDL_RenderCopyEx(renderer, tmp->sheet, &alRect, &al_rec, 0, NULL, 0);
    }
    SDL_RenderPresent(renderer);
}

void update(Player *pl, struct LinkedList *aliens) {
    int goDown = 0;
    if (border >= 265) {
        goDown = 1;
        border = 265;
        speed = abs(speed) * (-1);
    }
    else if (border <= 0) {
        goDown = 1;
        border = 0;
        speed = abs(speed);
    }
    border += speed;

    for (int i = 0; i < aliens->size; ++i) {
        struct Alien *tmp = *(struct Alien **) get(aliens, i);
        tmp->x += speed;
        tmp->y += 40 * goDown;
    }

}

int eventPoll(Player *pl, int dx) {
    int done = 0;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                done = 1;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: done = 1;
                }
                break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
        done = 1;
    }
    if (state[SDL_SCANCODE_LEFT]) {
        dx = -5;
        changeSpriteShip(pl, 0, -1);
    }
    else if (state[SDL_SCANCODE_RIGHT]) {
        dx = 5;
        changeSpriteShip(pl, 4, 1);
    }
    else {
        dx = 0;
        pl->currentSprite = 2;
    }
    if (pl->currentSprite == 4 || pl->currentSprite == 0)
        dx  = dx * 2;

    pl->x += dx;
    if (pl->x >= 844 || pl->x <= 0)
        pl->x -= dx;
    else
        pl->x += dx;
    return done;
}

int main(int argc, char* args[]) {
    SDL_Renderer *renderer = init();

    SDL_Surface *sheet = loadImage("../../resources/Ship.png");
    Player pl = {394, 645, 56, 70, 2, 0,
                 SDL_CreateTextureFromSurface(renderer, sheet)};
    SDL_FreeSurface(sheet);

    struct LinkedList *aliens = (struct LinkedList *) malloc(sizeof(struct LinkedList));
//    Alien *alien;
    getAliens(renderer, aliens);

    int done = 0;

    while(done != 1) {
        done = eventPoll(&pl, 0);
        update(&pl, aliens);
        draw(renderer, &pl, aliens);
        SDL_Delay(100);

    }

    list_destroy(aliens);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}

int list_example(int argc, char* args[]) {
    struct LinkedList list;
    createList(&list, sizeof(int), NULL);

    for (int i = 0; i < 6; ++i) {
        add(&list, &i);
    }

    delete_node(&list, 4);

    for (int j = 0; j < length(&list); ++j) {
        printf("getting: %i\n", *(int *) get(&list, j));
    }

    list_destroy(&list);
    return 0;
}