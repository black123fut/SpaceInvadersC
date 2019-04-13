#ifndef SPACEINVADERS_CONNECTION_H
#define SPACEINVADERS_CONNECTION_H

#include "../Sprites/Sprite.h"
#include <SDL_render.h>
#include <json-c/json_object.h>
#include <zconf.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#include "Ssocket.h"
#include <pthread.h>
#include "../JsonBuilder/JsonConstructor.h"

typedef void (* updateFunction)(Player *pl, struct LinkedList *aliens,
                               struct LinkedList *bullets,
                               struct LinkedList *shields,
                               SDL_Renderer *renderer,
                                const char *buffer);

typedef int (* eventPollFunction)(Player *pl, int dx,
                                    struct LinkedList *bullets,
                                    SDL_Renderer * renderer);

typedef void (* drawFunction)(SDL_Renderer *renderer, Player *pl,
                              struct LinkedList *aliens,
                              struct LinkedList *bullets,
                              struct LinkedList *shields,
                              SDL_Texture* typeText);

struct client_info {
    int sockno;
    char ip[INET_ADDRSTRLEN];
};

int connect_observer(updateFunction updateClient,
                     eventPollFunction eventPoll,
                     drawFunction draw,
                     Player *pl, int speed,
                     struct LinkedList *aliens,
                     struct LinkedList *bullets,
                     struct LinkedList *shields,
                     SDL_Renderer *renderer, bool *lose);

int connect_client_player(updateFunction updateClient,
                          eventPollFunction eventPoll,
                          drawFunction draw,
                          Player *pl, int speed,
                          struct LinkedList *aliens,
                          struct LinkedList *bullets,
                          struct LinkedList *shields,
                          SDL_Renderer *renderer,
                          struct game_info *game, bool *lose);

#endif //SPACEINVADERS_CONNECTION_H
