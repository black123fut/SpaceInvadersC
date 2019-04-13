#ifndef SPACEINVADERS_JSONCONSTRUCTOR_H
#define SPACEINVADERS_JSONCONSTRUCTOR_H

#include <stdio.h>
#include "../Sprites/Block.h"
#include "../DataStructures/Utilities.h"
#include "../Sprites/Sprite.h"
#include "../DataStructures/LinkedList.h"
#include <jmorecfg.h>
#include <json-c/json.h>

void gameInfoJson(struct game_info *game, json_object *jobj);
void typeClient(int type, json_object *jobj);
void playerJson(Player *pl, json_object *jobj);
void bulletsJson(struct LinkedList *blocks, json_object *jobj);
void blocksJson(struct LinkedList *blocks, json_object *jobj);
void aliensJson(struct LinkedList *aliens, json_object *jobj);
void speedJson(int speed, json_object *jobj);

#endif //SPACEINVADERS_JSONCONSTRUCTOR_H
