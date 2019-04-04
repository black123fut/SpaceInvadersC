#include "Utilities.h"

//width 900
//height 720

void free_alien(void *data) {
    free(*(struct Alien **)data);
}

void setAttributes(struct Alien *alien, int column, int index, int row) {
    alien->y = 75 + row * 32;

    if (row == 0) {
        alien->x = 16 + column * 60;
        alien->width = 24;
    }
    else if (row == 1 || row == 2) {
        alien->x = 11 + column * 60;
        alien->width = 33;
    }
    else {
        alien->x = 10 + column * 60;
        alien->width = 35;
    }

    alien->height = 24;
    alien->currentSprite = 0;
    alien->index = index;
    alien->row = row;
}

SDL_Surface *getAlienImage(int row) {
    if (row == 0) {
        return loadImage("../../resources/SquidAlien.png");
    }
    else if (row == 1 || row == 2) {
        return loadImage("../../resources/BasicAlien.png");
    }
    else {
        return loadImage("../../resources/FatAlien.png");
    }
}

void getAliens(SDL_Renderer *renderer, struct LinkedList *aliens) {
    int row = 0, column = 0, index = 0;
    createList(aliens, sizeof(struct Alien *), free_alien);

    while (row != 5) {
        SDL_Surface *sheet = getAlienImage(row);
        struct Alien *alien = (struct Alien *) malloc(sizeof(struct Alien));
        setAttributes(alien, column, index, row);
        alien->sheet = SDL_CreateTextureFromSurface(renderer, sheet);
        SDL_FreeSurface(sheet);
        add(aliens, &alien);

        printf("column: %i \n", column);
        column++;
        index++;
        if (column == 11) {
            column = 0;
            row++;
        }
    }
    printf("LLEgue");
}

void changeSpriteAliens(struct LinkedList *aliens, int *time) {
    *time = *time + 1;
    int timeaux = *time;
    if (timeaux > 10) {
        *time = 0;
    }
    if (timeaux % 10 == 0) {
        for (int i = 0; i < aliens->size; ++i) {
            struct Alien *tmp = *(struct Alien **) get(aliens, i);
            tmp->currentSprite = ++tmp->currentSprite % 2;
        }
    }
}

void changeSpriteShip(Player *pl, int max, int dir) {
    pl->time++;
    if (pl->time > 5) {
        pl->time = 0;
    }
    if ((pl->currentSprite != max) && (pl->time % 5 == 0)) {
        pl->currentSprite += dir;
        pl->time = 0;
    }
}

SDL_Renderer *init() {
    int width = 900;
    int height = 720;

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

