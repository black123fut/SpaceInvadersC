#include "JsonConstructor.h"


/**
 *
 *
 * En general todos escriben un Json
 * de un diferenet objeto.
 *
 *
 */


void gameInfoJson(struct game_info *game, json_object *jobj) {
    json_object_object_add(jobj,"Vidas", json_object_new_int(game->lifes));
    json_object_object_add(jobj,"Score", json_object_new_int(game->score));
}

void typeClient(int type, json_object *jobj) {
    json_object_object_add(jobj,"Client", json_object_new_int(type));
}

void speedJson(int speed, json_object *jobj) {
    json_object_object_add(jobj,"GlobalSpeed", json_object_new_int(speed));
}

void playerJson(Player *pl, json_object *jobj) {
    json_object *jPl = json_object_new_object();
    json_object_object_add(jPl, "x", json_object_new_int(pl->x));
    json_object_object_add(jPl, "y", json_object_new_int(pl->y));
    json_object_object_add(jPl, "w", json_object_new_int(pl->width));
    json_object_object_add(jPl, "h", json_object_new_int(pl->height));
    json_object_object_add(jPl, "cSpr", json_object_new_int(pl->currentSprite));
    json_object_object_add(jPl, "time", json_object_new_int(pl->time));
    json_object_object_add(jPl, "cooldown", json_object_new_int(pl->cooldown));

    json_object_object_add(jobj,"Player", jPl);

}

void aliensJson(struct LinkedList *aliens, json_object *jobj) {
    json_object *jarray = json_object_new_array();

    for (int i = 0; i < length(aliens); ++i) {
        struct Alien *tmp = *(struct Alien **) get(aliens, i);
        
        json_object *jalien = json_object_new_object();
        json_object_object_add(jalien, "x", json_object_new_int(tmp->x));
        json_object_object_add(jalien, "y", json_object_new_int(tmp->y));
        json_object_object_add(jalien, "w", json_object_new_int(tmp->width));
        json_object_object_add(jalien, "h", json_object_new_int(tmp->height));
        json_object_object_add(jalien, "row", json_object_new_int(tmp->row));
        json_object_object_add(jalien, "ind", json_object_new_int(tmp->index));
        json_object_object_add(jalien, "cSpr", json_object_new_int(tmp->currentSprite));

        json_object_array_add(jarray, jalien);
    }

    json_object_object_add(jobj,"Aliens", jarray);

}

void blocksJson(struct LinkedList *shields, json_object *jobj) {
    json_object *jShieldsArray = json_object_new_array();

    for (int l = 0; l < shields->size; ++l) {
        struct Block *tmp = *(struct Block **) get(shields, l);
        json_object *jblock = json_object_new_object();
        json_object *jarray = json_object_new_array();

        for (int i = 0; i < 14; ++i) {
            json_object *jrow = json_object_new_object();
            char *row = malloc(17);
            char *head = row;
            for (int j = 0; j < 16; ++j) {

                if (tmp->state[i][j] == 0) {
                    *row = '0';
                }
                else {
                    *row = '1';
                }
                row++;
            }

            char *key = (char*) malloc(3);
            sprintf(key, "r%i", i);

            json_object_object_add(jrow, key, json_object_new_string(head));
            json_object_array_add(jarray, jrow);
        }
        json_object_object_add(jblock, "Block", jarray);
        json_object_array_add(jShieldsArray, jblock);
    }
    json_object_object_add(jobj, "Shields", jShieldsArray);

}

void bulletsJson(struct LinkedList *bullets, json_object *jobj) {
    json_object *jarray = json_object_new_array();

    for (int i = 0; i < length(bullets); ++i) {
        struct Bullet *tmp = *(struct Bullet **) get(bullets, i);

        json_object *jbullet = json_object_new_object();
        json_object_object_add(jbullet, "x", json_object_new_int(tmp->x));
        json_object_object_add(jbullet, "y", json_object_new_int(tmp->y));
        json_object_object_add(jbullet, "w", json_object_new_int(tmp->width));
        json_object_object_add(jbullet, "h", json_object_new_int(tmp->height));
        json_object_object_add(jbullet, "cSpr", json_object_new_int(tmp->currentSprite));
        json_object_object_add(jbullet, "dir", json_object_new_int(tmp->direction));

        json_object_array_add(jarray, jbullet);
    }

    json_object_object_add(jobj,"Bullets", jarray);
}