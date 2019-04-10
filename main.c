#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "DataStructures/LinkedList.h"
#include "Sprites/Block.h"
#include "Sprites/Sprite.h"
#include "DataStructures/Utilities.h"
#include "JsonBuilder/JsonConstructor.h"


int globalTime = 0, border = 10, speed = 1;
bool servidor = true;

void draw(SDL_Renderer *renderer, Player *pl,
                    struct LinkedList *aliens,
                    struct LinkedList *bullets,
                    struct LinkedList *shields) {

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //El jugador
    SDL_Rect srcRect = { pl->width * pl->currentSprite, 0, pl->width, pl->height};
    SDL_Rect rect = { pl->x, pl->y, pl->width, pl->height };
    SDL_RenderCopyEx(renderer, pl->sheet, &srcRect, &rect, 0, NULL, 0);

    //Los Aliens
    if (servidor)
        changeSpriteAliens(aliens, &globalTime);
    for (int i = 0; i < aliens->size; ++i) {
        struct Alien *tmp = *(struct Alien **) get(aliens, i);
        SDL_Rect alRect = { tmp->width * tmp->currentSprite, 0, tmp->width, tmp->height};
        SDL_Rect al_rec = { tmp->x, tmp->y, tmp->width, tmp->height };
        SDL_RenderCopyEx(renderer, tmp->sheet, &alRect, &al_rec, 0, NULL, 0);
    }

    //Las Balas del alien y jugador
    if (bullets->size > 0) {
        if (servidor)
            changeSpriteBullets(bullets, &globalTime);
        for (int j = 0; j < length(bullets); ++j) {
            struct Bullet *tmp = *(struct Bullet **) get(bullets, j);
            if (tmp != NULL) {
                SDL_Rect alRect = { tmp->width * tmp->currentSprite, 0, tmp->width, tmp->height};
                SDL_Rect al_rec = { tmp->x, tmp->y, tmp->width, tmp->height };
                SDL_RenderCopyEx(renderer, tmp->sheet, &alRect, &al_rec, 0, NULL, 0);
            }
        }
    }

    //Dibuja bloques

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    //Dibuja los bloques del escudo
    for (int l = 0; l < shields->size; ++l) {
        struct Block *tmp = *(struct Block **) get(shields, l);
        int xPos = 100 + l * (80 + 126);

        for (int i = 0; i < 14; ++i) {
            for (int j = 0; j < 16; ++j) {
                if (tmp->state[i][j] != 0) {
                    SDL_Rect rectM = {xPos + j * 5, 530 + i * 5, 5, 5};
                    SDL_RenderFillRect(renderer, &rectM);
                }
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void updateServer(Player *pl, struct LinkedList *aliens,
                  struct LinkedList *bullets,
                  struct LinkedList *shields,
                  SDL_Renderer *renderer) {

    int goDown = 0;

    if (pl->cooldown > 0) {
        pl->cooldown--;
    }

    //Cuando los aliens tocan los limites de la pantalla
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

    //Selecciona uhn alien random para disparar
    if (globalTime % 50 == 0 && globalTime != 0) {
        struct Alien *alien = *(struct Alien **) get(aliens, rand() % length(aliens));
        addBulletAlien(bullets, alien, renderer);
    }

    //Mueve los aliens
    for (int i = 0; i < aliens->size; ++i) {
        struct Alien *tmp = *(struct Alien **) get(aliens, i);
        tmp->x += speed;
        tmp->y += 40 * goDown;
    }

    //Mueve balas y Balas fueras del limite
    for (int k = 0; k < length(bullets); ++k) {
        struct Bullet *tmp = *(struct Bullet **) get(bullets, k);
        tmp->y += 15 * tmp->direction;

        if (tmp->y <= 0 || tmp->y >= 730) {
            delete_node(bullets, k, "Alien Bullet");
        }
    }

    //Colisiones
    if (bullets->size > 0) {
        for (int l = 0; l < aliens->size; ++l) {
            struct Alien *alientmp = *(struct Alien **) get(aliens, l);
            for (int i = 0; i < length(bullets); ++i) {

                struct Bullet *bullettmp = *(struct Bullet **) get(bullets, i);
                if (bullettmp->direction == -1 && checkCollision(bullettmp, alientmp)) {
                    delete_node(bullets, i, "Player Bullet");
                    delete_node(aliens, l, "Alien");
                }
            }
        }
    }

    //Destruccion de los escudos
    if (bullets->size > 0) {
        searchCollision(bullets, shields);
    }
}

int eventPoll(Player *pl, int dx, struct LinkedList *bullets, SDL_Renderer * renderer) {
    int done = 0;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                done = 1;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: done = 1; break;
                }
                break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
        done = 1;
    }
    //
    if (state[SDL_SCANCODE_LEFT]) {
        dx = -5;
        changeSpriteShip(pl, 0, -1);
    }
    else if (state[SDL_SCANCODE_RIGHT]) {
        dx = 5;
        changeSpriteShip(pl, 4, 1);
    }
    //Disparo.
    else if (state[SDL_SCANCODE_Z] && pl->cooldown == 0) {
        addBulletPlayer(bullets, pl, renderer);
        pl->cooldown = 10;
    }
    else {
        dx = 0;
        pl->currentSprite = 2;
    }

    if (pl->currentSprite == 4 || pl->currentSprite == 0)
        dx  = dx * 2;

    pl->x += dx;
    //Limites de la pantalla
    if (pl->x >= 844 || pl->x <= 0)
        pl->x -= dx;
    else
        pl->x += dx;
    return done;
}

int main(int argc, char* args[]) {
    SDL_Renderer *renderer = init();

    SDL_Surface *sheet = loadImage("../../resources/Ship.png");
    Player pl = {394, 645, 56, 70, 2, 0, 0,
                 SDL_CreateTextureFromSurface(renderer, sheet)};
    SDL_FreeSurface(sheet);

    struct LinkedList *aliens = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    getAliens(renderer, aliens);

    struct LinkedList *bullets = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    createList(bullets, sizeof(struct Bullet *), free_bullet);

    struct LinkedList *shields = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    createList(shields, sizeof(struct Block *), free_block);
    generateShields(shields);

    addBulletPlayer(bullets, &pl, renderer);

    json_object *jobj = json_object_new_object();
    //aliensJson(aliens, jobj);
    //blocksJson(shields, jobj);
    //bulletsJson(bullets, jobj);

    playerJson(&pl, jobj);
    speedJson(speed, jobj);

    int done = 0;

    while(done != 1) {

        updateServer(&pl, aliens, bullets, shields, renderer);
        draw(renderer, &pl, aliens, bullets, shields);
        done = eventPoll(&pl, 0, bullets, renderer);
        SDL_Delay(50);
    }

    for (int i = 0; i < 4; ++i) {
        struct Block *tmp = *(struct Block **) get(shields, i);
        list_destroy(tmp->shield);
    }
    list_destroy(bullets);
    list_destroy(aliens);
    list_destroy(shields);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}

int list_example(int argc, char* args[]) {
    struct LinkedList *list = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    createList(list, sizeof(int), NULL);

    for (int i = 0; i < 6; ++i) {
        add(list, &i);
    }

    //delete_node(&list, 4);

    for (int j = 0; j < length(list); ++j) {
        printf("getting: %i\n", *(int *) get(list, j));
    }

    clear_list(list);

    for (int i = 4; i < 8; ++i) {
        add(list, &i);
    }

    for (int j = 0; j < length(list); ++j) {
        printf("getting: %i\n", *(int *) get(list, j));
    }

    list_destroy(list);

    return 0;
}