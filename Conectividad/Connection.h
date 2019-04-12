#ifndef SPACEINVADERS_CONNECTION_H
#define SPACEINVADERS_CONNECTION_H

#include "../Sprites/Sprite.h"
#include <SDL_render.h>
#include <json-c/json_object.h>
#include <zconf.h>
#include "Ssocket.h"
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

int connect_server(updateFunction updateServer,
                   updateFunction updateClient,
                    eventPollFunction eventPoll,
                    drawFunction draw,
                    Player *pl, int speed,
                    struct LinkedList *aliens,
                    struct LinkedList *bullets,
                    struct LinkedList *shields,
                    SDL_Renderer *renderer);

int connect_client(updateFunction updateClient,
                   eventPollFunction eventPoll,
                   drawFunction draw,
                   Player *pl, int speed,
                   struct LinkedList *aliens,
                   struct LinkedList *bullets,
                   struct LinkedList *shields,
                   SDL_Renderer *renderer);

#endif //SPACEINVADERS_CONNECTION_H
