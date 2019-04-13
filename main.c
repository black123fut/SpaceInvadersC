#include <stdio.h>
#include <signal.h>
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
#include "MenuScene/Menu.h"


int globalTime = 0, border = 10, speed = 1, isObserver;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
const int puerto = 8080;
Player pl;
struct LinkedList *clients;
struct LinkedList *aliens;
struct LinkedList *bullets;
struct LinkedList *shields;
struct game_info game;
SDL_Renderer *renderer;

int n = 0;
bool isServer = true, playerConnected = false, serverConnected = false, lose = false;

void handler(int s) {
    printf("Caught SIGPIPE\n");
}

/**
 * Funcion que dibuja la ventana con las animaciones
 */
void draw(SDL_Renderer *renderer, Player *pl,
          struct LinkedList *aliens,
          struct LinkedList *bullets,
          struct LinkedList *shields,
          SDL_Texture* typeText) {

    //Verifica si perdio, si no sigue dibujando el juego
    if (!lose) {
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

        SDL_RenderClear(renderer); //Limpiala pantalla

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        //El jugador
        SDL_Rect srcRect = { pl->width * pl->currentSprite, 0, pl->width, pl->height};
        SDL_Rect rect = { pl->x, pl->y, pl->width, pl->height };
        SDL_RenderCopyEx(renderer, pl->sheet, &srcRect, &rect, 0, NULL, 0);

        //Los Aliens

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

        //Escribe el texto
        char scoreT[10];
        sprintf(scoreT, "Score: %i", game.score);
        SDL_Texture* scoreText = loadText(renderer, scoreT);
        SDL_Rect score_rect = {750, 10, 150, 50};
        SDL_RenderCopy(renderer, scoreText, NULL, &score_rect);
        SDL_DestroyTexture(scoreText);

        //Escribr las vida en pantalla
        char lifesT[10];
        sprintf(lifesT, "Vidas: %i", game.lifes);
        SDL_Texture* lifesText = loadText(renderer, lifesT);
        SDL_Rect lifes_rect = {550, 10, 150, 50};
        SDL_RenderCopy(renderer, lifesText, NULL, &lifes_rect);
        SDL_DestroyTexture(lifesText);

        SDL_Rect Message_rect = {0, 0, 200, 50};
        SDL_RenderCopy(renderer, typeText, NULL, &Message_rect);

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
    }
    else {
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        char loseT[] = "You Lose";
        SDL_Texture* scoreText = loadText(renderer, loseT);
        SDL_Rect score_rect = {275, 260, 350, 200};
        SDL_RenderCopy(renderer, scoreText, NULL, &score_rect);
        SDL_DestroyTexture(scoreText);
    }

    SDL_RenderPresent(renderer);
}

/**
 * Actualiza los datos del cliente con los del servidor
 */
void updateClient(Player *pl, struct LinkedList *aliens,
                  struct LinkedList *bullets,
                  struct LinkedList *shields,
                  SDL_Renderer *renderer,
                  const char *buffer) {

    int typeRes;
    //Json---------------------------------------------
    struct json_object *parsed_json;
    struct json_object *tipo;
    struct json_object *jugador;
    struct json_object *temp_jug;
    struct json_object *balas;
    struct json_object *bala;
    struct json_object *extraterrestres;
    struct json_object *extraterrestre;
    struct json_object *escudos;
    struct json_object *score;
    struct json_object *vidas;
    struct json_object *state;

    parsed_json = json_tokener_parse(buffer);

    if (parsed_json == NULL) {
        typeRes = 1;
    }
    else {
        //Prepara la infomacion en las variables
        json_object_object_get_ex(parsed_json,"Client", &tipo);

        json_object_object_get_ex(parsed_json,"Player", &jugador);
        json_object_object_get_ex(parsed_json,"Bullets", &balas);
        json_object_object_get_ex(parsed_json,"Aliens", &extraterrestres);
        json_object_object_get_ex(parsed_json,"Shields", &escudos);
        json_object_object_get_ex(parsed_json,"Score", &score);
        json_object_object_get_ex(parsed_json,"Vidas", &vidas);
        json_object_object_get_ex(parsed_json,"State", &state);


        //Pasa la informacion del Json a las varibles del juego.
        jugador = json_object_object_get(parsed_json,"Player");

        game.score = json_object_get_int(score);
        game.lifes = json_object_get_int(vidas);

        lose = json_object_get_int(state);

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

        //Lee los escudos del json
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
}

/**
 *  Mueve el jugador y ademas hace que dispare
 */
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
        pl->cooldown = 20;
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
    Player player = {394, 645, 56, 70, 2, 0, 0,
                     SDL_CreateTextureFromSurface(renderer, sheet)};
    SDL_FreeSurface(sheet);

    pl = player;

    int selected = menu(playerConnected, serverConnected, renderer);

    // 0 es Jugador
    // 1 es obserdavador

    if (selected == 2) {
        isServer = false;
        isObserver = 0;
    }
    else {
        isServer = false;
        isObserver = 1;
    }

    game = (struct game_info) {3, 0};
    aliens = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    getAliens(renderer, aliens);

    bullets = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    createList(bullets, sizeof(struct Bullet *), free_bullet);

    shields = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    createList(shields, sizeof(struct Block *), free_block);
    generateShields(shields);

    signal(SIGPIPE, handler);

    //Cliente Observador
    if (isObserver == 1) {
        pthread_mutex_lock(&mutex);
        connect_observer(updateClient, NULL, draw, &pl, globalTime, aliens, bullets, shields, renderer, &lose);
        pthread_mutex_unlock(&mutex);
        }
    //Cliente Jugador
    else {
        playerConnected = true;
        pthread_mutex_lock(&mutex);
        connect_client_player(updateClient, eventPoll, draw, &pl, globalTime, aliens, bullets, shields, renderer, &game, &lose);
        pthread_mutex_unlock(&mutex);
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