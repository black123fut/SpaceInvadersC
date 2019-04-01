#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "DataStructures/LinkedList.h"
#include "Sprites/Sprite.h"

int width = 1280;
int height = 720;

SDL_Renderer *init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Cannot initialise SDL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow(
            "SpaceInvaders",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
        exit(1);
    }

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );

    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (renderer == NULL) {
        fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0) {
        fprintf(stderr, "SDL_BlendMode failed: %s\n", SDL_GetError());
        exit(1);
    };

    return renderer;
}

SDL_Surface *loadImage(char* path) {
    SDL_Surface *sheet = IMG_Load(path);
    if(!sheet) {
        printf("Cannot find sheet\n");
        return NULL;
    }
    return sheet;
}

void draw(SDL_Renderer *renderer, Player *pl) {

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //El jugador
    SDL_Rect srcRect = { 70*2, 0, 70, 87 };
    SDL_Rect rect = { pl->x, pl->y, 40, 50 };
    SDL_RenderCopyEx(renderer, pl->sheet, &srcRect, &rect, 0, NULL, 0);


    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
    SDL_Renderer *renderer = init();

    SDL_Surface *sheet = loadImage("/Users/isaacbenavides/CLionProjects/SpaceInvaders/resources/Ship.png");
    Player p1 = {600, 550, 70, 87,
                 SDL_CreateTextureFromSurface(renderer, sheet)};
    SDL_FreeSurface(sheet);
    
    int dx = 0;
    int dy = 0;

    int done = 0;

    for (;;) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    goto quit;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE: goto quit;
                        case SDLK_UP: dy = -1; break;
                        case SDLK_RIGHT: dx = 1; break;
                        case SDLK_DOWN: dy = 1; break;
                        case SDLK_LEFT: dx = -1; break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (e.key.keysym.sym) {
                        case SDLK_UP: dy = 0; break;
                        case SDLK_RIGHT: dx = 0; break;
                        case SDLK_DOWN: dy = 0; break;
                        case SDLK_LEFT: dx = 0; break;
                    }
                    break;
            }
        }
        p1.x += dx;
        p1.y += dy;

        draw(renderer, &p1);
    }
    quit:
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


























