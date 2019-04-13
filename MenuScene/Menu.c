#include "Menu.h"


void drawMenu(SDL_Renderer *renderer, SDL_Texture *background, int current, SDL_Texture* clientPlayerText, SDL_Texture* observerText,
            bool playerConnected, bool serverConnected) {

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //Fondo
    SDL_RenderCopy(renderer, background, NULL, NULL);

    //Rectangulos de los textos
    SDL_Rect rect2 = {350, 420, 200, 70};
    SDL_Rect rect3 = {350, 540, 200, 70};

    //Rellena los rectangulos
    if (current == 1)
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);

    if (!playerConnected) {
        SDL_RenderFillRect(renderer, &rect2);
        SDL_RenderCopy(renderer, clientPlayerText, NULL, &rect2);
    }

    if (current == 2)
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);

    SDL_RenderFillRect(renderer, &rect3);

    //Escribe los textos
    SDL_RenderCopy(renderer, observerText, NULL, &rect3);

    SDL_RenderPresent(renderer);
}

int menu(bool playerConnected, bool serverConnected, SDL_Renderer *renderer) {
    int current = 1, done = 1;
    SDL_Surface *background = loadImage("../../resources/Menusp.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, background);
    SDL_FreeSurface(background);

    SDL_Texture* clientPlayerText = loadText(renderer, "Jugador");
    SDL_Texture* observerText = loadText(renderer, "Observador");


    while (done != 0) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE: done = 0; break;
                        case SDLK_UP:
                            if (current == 1)
                                current = 2;
                            else
                                current--;
                            break;

                        case SDLK_DOWN:
                            if (current == 2)
                                current = 1;
                            else
                                current++;
                            break;
                        case SDLK_SPACE:
                            return current;
                    }
                    break;
            }
        }
        drawMenu(renderer, texture, current, clientPlayerText, observerText,
                playerConnected, serverConnected);
        SDL_Delay(60);
    }
}