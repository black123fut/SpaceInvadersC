#include "Connection.h"

/**
 * Recibe los datos del servidor para ser observados en la ventana
 */
int connect_observer(updateFunction updateClient,
                     eventPollFunction eventPoll,
                     drawFunction draw,
                     Player *pl, int speed,
                     struct LinkedList *aliens,
                     struct LinkedList *bullets,
                     struct LinkedList *shields,
                     SDL_Renderer *renderer, bool *lose) {

    const int puerto = 8080;
    int done = 0;
    int cliente = openSocket();
    if(cliente == -1) {
        perror("Error al abrir socket");
        return 1;
    }
    if(bindPortCliente(cliente,puerto) == -1)
        return 1;

//    printf("Estoy escuchando\n");
    listen(cliente, 100);

//    char buffer[10000];
    char len[5];
    int largo;

    SDL_Texture* textClient = loadText(renderer, "Cliente-Observador");

    char opcode[2] = "1";
    send(cliente, opcode, 2, 0);

    while (done != 1) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE: done = 1; break;
                    }
                    break;
            }
        }

        ///////////////// Recibe el largo  |||||||||||||||||||||||||||
        int bytesRecibidos = recv(cliente, len, 5, 0);

        if (bytesRecibidos < 0) {
            perror("Pus se deconecto we cliente2");
            continue;
        }

        len[bytesRecibidos] = '\0';
        //printf("Me llegaron %d bytes con el mensaje: %s \n", bytesRecibidos, len);

        largo = atoi(len);
        usleep(50);
        printf("largo: %d\n",largo);
        // ----------------------------------------------------------

        ///////////////// Envia provicional |||||||||||||||||||||||||||
        char test[3] = "23";
        send(cliente, test, 3, 0);

        ///////////////// Recibe el mensaje |||||||||||||||||||||||||
        printf("\nLlego1\n");
        char buffer[largo];
        bytesRecibidos = recv(cliente, buffer, largo, 0);
        printf("\nLlego2 %i\n", bytesRecibidos);
        if (bytesRecibidos < 0) {
            perror("Pus se deconecto we cliente1");
            continue;
        }


        buffer[bytesRecibidos] = '\0';
        printf("Me llegaron %d bytes con el mensaje: %s \n", bytesRecibidos, buffer);
        usleep(50);
        // ----------------------------------------------------------

        updateClient(pl, aliens, bullets, shields,renderer, buffer);
        draw(renderer, pl, aliens, bullets, shields, textClient);

        ///////////////// Envia provicional |||||||||||||||||||||||||||
        send(cliente, test, 3, 0);

        SDL_Delay(50);

    }
}

/**
 * Recibe la informacion de la partida del servidor y ademas permite controlar la nave
 */
int connect_client_player(updateFunction updateClient,
                          eventPollFunction eventPoll,
                          drawFunction draw,
                          Player *pl, int speed,
                          struct LinkedList *aliens,
                          struct LinkedList *bullets,
                          struct LinkedList *shields,
                          SDL_Renderer *renderer,
                          struct game_info *game, bool *lose) {

    const int puerto = 8080;
    int done = 0;
    int cliente = openSocket();
    if(cliente == -1) {
        perror("Error al abrir socket");
        return 1;
    }
    if(bindPortCliente(cliente,puerto) == -1)
        return 1;

//    printf("Estoy escuchando\n");
    listen(cliente, 100);

//    char buffer[10000];
    char len[5];
    int largo;

    SDL_Texture* textClient= loadText(renderer, "Cliente-Jugador");

    char opcode[2] = "2";
    send(cliente, opcode, 2, 0);

    while (done != 1) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE: done = 1; break;
                    }
                    break;
            }
        }
        ///////////////// Recibe el largo  |||||||||||||||||||||||||||
        int bytesRecibidos = recv(cliente, len, 5, 0);

        if (bytesRecibidos < 0) {
            perror("Pus se deconecto we cliente2");
            continue;
        }

        len[bytesRecibidos] = '\0';
        //printf("Me llegaron %d bytes con el mensaje: %s \n", bytesRecibidos, len);

        largo = atoi(len);
        usleep(50);
        printf("largo: %d\n",largo);
        // ----------------------------------------------------------

        //Envia provicional
        char test[3] = "23";
        send(cliente, test, 3, 0);

        ///////////////// Recibe el mensaje |||||||||||||||||||||||||
        printf("\nLlego1\n");
        char buffer[largo];
        bytesRecibidos = recv(cliente, buffer, largo, 0);
        printf("\nLlego2 %i\n", bytesRecibidos);
        if (bytesRecibidos < 0) {
            perror("Pus se deconecto we cliente1");
            continue;
        }
        buffer[bytesRecibidos] = '\0';
        printf("Me llegaron %d bytes con el mensaje: %s \n", bytesRecibidos, buffer);
        usleep(50);
        // ----------------------------------------------------------

        updateClient(pl, aliens, bullets, shields,renderer,buffer);
        draw(renderer, pl, aliens, bullets, shields, textClient);
        done = eventPoll(pl, 0, bullets, renderer);

        ///////////////// Crea el mensaje |||||||||||||||||||||||||

        json_object *jobj = json_object_new_object();
        typeClient(0, jobj);
        gameInfoJson(game, jobj);
        playerJson(pl, jobj);
        bulletsJson(bullets, jobj);
//        aliensJson(aliens, jobj);
//        blocksJson(shields, jobj);
//        speedJson(speed, jobj);

        // --------------------------------------------------------


        ///////////////// Envia el largo |||||||||||||||

        char const *mensaje = json_object_to_json_string(jobj);
        int length = strlen(mensaje);
        char messageLength[4];
        if (done != 1)
            sprintf(messageLength, "%d", length);
        else
            strcpy(messageLength, "quit");

        //printf("\nNesasf size: %lu\n", strlen(messageLength));
        int response = send(cliente, messageLength, strlen(messageLength), 0);
        if (response < 0) {
            perror("message not sent1");
            exit(1);
        }
        printf("A server length: %s\n", messageLength);
        usleep(50);
        // ---------------------------------------------

        if (done == 1)
            break;

        // Recibe provisional
        char tesc3[3];
        int test2 = recv(cliente, tesc3, 3, 0);

        //////////////// Envia el mensaje Json |||||||||||||||

        response = send(cliente,mensaje,strlen(mensaje), 0);
        if (response < 0) {
            perror("message not sent1");
            exit(1);
        }
        usleep(50);
        //printf("A server: %s", mensaje);
        // ----------------------------------------------------
        SDL_Delay(50);

    }
    return 0;
}