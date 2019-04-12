#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <SDL_ttf.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "DataStructures/LinkedList.h"
#include "Sprites/Block.h"
#include "Sprites/Sprite.h"
#include "DataStructures/Utilities.h"
#include "Conectividad/Ssocket.h"
#include "JsonBuilder/JsonConstructor.h"
#include "Conectividad/Connection.h"


int globalTime = 0, border = 10, speed = 1;
bool isServer = true;

void draw(SDL_Renderer *renderer, Player *pl,
                    struct LinkedList *aliens,
                    struct LinkedList *bullets,
                    struct LinkedList *shields,
                    SDL_Texture* typeText) {

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //El jugador
    SDL_Rect srcRect = { pl->width * pl->currentSprite, 0, pl->width, pl->height};
    SDL_Rect rect = { pl->x, pl->y, pl->width, pl->height };
    SDL_RenderCopyEx(renderer, pl->sheet, &srcRect, &rect, 0, NULL, 0);

    //Los Aliens
    if (!isServer)
        changeSpriteAliens(aliens, &globalTime);
    for (int i = 0; i < aliens->size; ++i) {
        struct Alien *tmp = *(struct Alien **) get(aliens, i);
        SDL_Rect alRect = { tmp->width * tmp->currentSprite, 0, tmp->width, tmp->height};
        SDL_Rect al_rec = { tmp->x, tmp->y, tmp->width, tmp->height };
        SDL_RenderCopyEx(renderer, tmp->sheet, &alRect, &al_rec, 0, NULL, 0);
    }

    //Las Balas del alien y jugador
    if (bullets->size > 0) {
        if (!isServer)
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

    if (!isServer) {
        SDL_Rect Message_rect = {0, 0, 100, 50};
        SDL_RenderCopy(renderer, typeText, NULL, &Message_rect);
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
                  SDL_Renderer *renderer,
                  const char *buffer) {

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

void updateClient(Player *pl, struct LinkedList *aliens,
                    struct LinkedList *bullets,
                    struct LinkedList *shields,
                    SDL_Renderer *renderer,
                    const char *buffer) {

    //Json---------------------------------------------
    struct json_object *parsed_json;
    struct json_object *jugador;
    struct json_object *temp_jug;
    struct json_object *balas;
    struct json_object *bala;
    struct json_object *extraterrestres;
    struct json_object *extraterrestre;
    struct json_object *escudos;
    struct json_object *escudo;

    parsed_json = json_tokener_parse(buffer);

    json_object_object_get_ex(parsed_json,"Player", &jugador);
    json_object_object_get_ex(parsed_json,"Bullets", &balas);
    json_object_object_get_ex(parsed_json,"Aliens", &extraterrestres);
    json_object_object_get_ex(parsed_json,"Shields", &escudos);
    jugador = json_object_object_get(parsed_json,"Player");

    json_object_object_get_ex(jugador,"x",&temp_jug);
    pl->x = json_object_get_int(temp_jug);
    json_object_object_get_ex(jugador,"y",&temp_jug);
    pl->y = json_object_get_int(temp_jug);
    json_object_object_get_ex(jugador,"w",&temp_jug);
    pl->width = json_object_get_int(temp_jug);
    json_object_object_get_ex(jugador,"h",&temp_jug);
    pl->height = json_object_get_int(temp_jug);
    json_object_object_get_ex(jugador,"cSpr",&temp_jug);
    pl->currentSprite = json_object_get_int(temp_jug);
    json_object_object_get_ex(jugador,"time",&temp_jug);
    pl->time = json_object_get_int(temp_jug);
    json_object_object_get_ex(jugador,"cooldown",&temp_jug);
    pl->cooldown = json_object_get_int(temp_jug);


    int numBalas = json_object_array_length(balas);
    printf("Numero de Balas:  %i\n",numBalas);
    clear_list(bullets);
    for (int i = 0; i < numBalas; i++) {
        bala = json_object_array_get_idx(balas, i);
        struct json_object *temp_bala;
        struct Bullet *tmp = (struct Bullet *) malloc(sizeof(struct Bullet));
        json_object_object_get_ex(bala, "x", &temp_bala);
        tmp->x = json_object_get_int(temp_bala);
        json_object_object_get_ex(bala, "y", &temp_bala);
        tmp->y = json_object_get_int(temp_bala);
        json_object_object_get_ex(bala, "w", &temp_bala);
        tmp->width = json_object_get_int(temp_bala);
        json_object_object_get_ex(bala, "h", &temp_bala);
        tmp->height = json_object_get_int(temp_bala);
        json_object_object_get_ex(bala, "cSpr", &temp_bala);
        tmp->currentSprite = json_object_get_int(temp_bala);
        json_object_object_get_ex(bala, "dir", &temp_bala);
        tmp->direction = json_object_get_int(temp_bala);
        SDL_Surface *sheet = getBulletImage(tmp->direction);
        tmp->sheet = SDL_CreateTextureFromSurface(renderer, sheet);
        SDL_FreeSurface(sheet);
        add(bullets, &tmp);
    }

    int numAliens = json_object_array_length(extraterrestres);
    printf("Numero de Aliens:  %i\n",numAliens);
    clear_list(aliens);
    for (int i = 0; i < numAliens; i++) {
        extraterrestre = json_object_array_get_idx(extraterrestres, i);
        struct json_object *temp_alien;
        struct Alien *tmp = (struct Alien *) malloc(sizeof(struct Alien));
        json_object_object_get_ex(extraterrestre, "x", &temp_alien);
        tmp->x = json_object_get_int(temp_alien);
        json_object_object_get_ex(extraterrestre, "y", &temp_alien);
        tmp->y = json_object_get_int(temp_alien);
        json_object_object_get_ex(extraterrestre, "w", &temp_alien);
        tmp->width = json_object_get_int(temp_alien);
        json_object_object_get_ex(extraterrestre, "h", &temp_alien);
        tmp->height = json_object_get_int(temp_alien);
        json_object_object_get_ex(extraterrestre, "row", &temp_alien);
        tmp->row = json_object_get_int(temp_alien);
        json_object_object_get_ex(extraterrestre, "ind", &temp_alien);
        tmp->index = json_object_get_int(temp_alien);
        json_object_object_get_ex(extraterrestre, "cSpr", &temp_alien);
        tmp->currentSprite = json_object_get_int(temp_alien);
        SDL_Surface *sheet = getAlienImage(tmp->row);
        tmp->sheet = SDL_CreateTextureFromSurface(renderer, sheet);
        SDL_FreeSurface(sheet);
        add(aliens, &tmp);
    }

    //Leer
    json_object *blockJ, *shieldsArr, *blockArr;

    for (int i = 0; i < json_object_array_length(escudos); ++i) {
        shieldsArr = json_object_array_get_idx(escudos, i);
        struct Block *tmp = *(struct Block **) get(shields, i);

        json_object_object_get_ex( shieldsArr, "Block", &blockJ );

        for (int j = 0; j < json_object_array_length(blockJ); ++j) {
            blockArr = json_object_array_get_idx(blockJ, j);

            json_object_object_foreach(blockArr, key, val) {
                const char *row;
                row = json_object_get_string(val);
                for(int k = 0; k < strlen(row); k++){
                    tmp->state[j][k] = row[k] - '0';
                }
            }
        }
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

    isServer = true;
    //Servidor
    if (isServer) {
        connect_server(updateServer, updateClient, NULL, draw, &pl, globalTime, aliens, bullets, shields, renderer);
    }
    else {
        connect_client(updateClient, eventPoll, draw, &pl, globalTime, aliens, bullets, shields, renderer);
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