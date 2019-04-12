#include "Connection.h"

const int puerto = 8080;

int connect_server(updateFunction updateServer,
                   updateFunction updateClient,
                    eventPollFunction eventPoll,
                    drawFunction draw,
                    Player *pl, int speed,
                    struct LinkedList *aliens,
                    struct LinkedList *bullets,
                    struct LinkedList *shields,
                    SDL_Renderer *renderer) {

    int done = 0;
    int servidor = openSocket();

    int activado = 1;
    setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

    if (bindPortServidor(servidor, 8080) == -1)
        return 1;

    printf("Estoy escuchando\n");
    listen(servidor, 100);

    //-------------------------------------------------------

    struct sockaddr_in direccionCliente;
    unsigned int tamanoDireccion;
    int cliente = accept(servidor, (void *) &direccionCliente, &tamanoDireccion);

    printf("Recibi una conexion en %d!!\n", cliente);

    //---------------------------------------------------
//            usleep(100);

    char buffer[12000];
    char len[5];
    int largo;

    while (done != 1) {
        updateServer(pl, aliens, bullets, shields, renderer, NULL);

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
        //draw(renderer, pl, aliens, bullets, shields, NULL);
        //done = eventPoll(pl, 0, bullets, renderer);

        ///////////////// Crea el mensaje |||||||||||||||||||||||||

        json_object *jobj = json_object_new_object();
        playerJson(pl, jobj);
        aliensJson(aliens, jobj);
        bulletsJson(bullets, jobj);
        blocksJson(shields, jobj);
        speedJson(speed, jobj);

        // --------------------------------------------------------

        char const *mensaje = json_object_to_json_string(jobj);

        ///////////////// Envia el largo |||||||||||||||||||||
        int length = strlen(mensaje);
        printf("\nlargo de de de %i \n", length);
        char messageLength[5];
        sprintf(messageLength, "%d", length);

        printf("\nlargo de men: %lu\n", strlen(messageLength));

        send(cliente, messageLength, strlen(messageLength), 0);
        usleep(100);
        // ---------------------------------------------------


        ///////////////// Envia el mensaje Json |||||||||||||||

        send(cliente,mensaje,strlen(mensaje),0);

        // ----------------------------------------------------

        ///////////////// Recibe el largo |||||||||||||||||||||
        int bytesRecibidos = recv(cliente, len, 5, 0);

        if (bytesRecibidos <= 0) {
            perror("Pus se deconecto we server");
            return 1;
        }

        len[bytesRecibidos] = '\0';
        printf("Me llegaron %d bytes con el mensaje: %s \n", bytesRecibidos, len);

        largo = atoi(len);
        printf("largo: %d\n",largo);
        // ---------------------------------------------------


        ///////////////// Recibe el mensaje |||||||||||||||||||||||||
        bytesRecibidos = recv(cliente, buffer, largo, 0);
        if (bytesRecibidos <= 0) {
            perror("Pus se deconecto we cliente1");
            break;
        }
        buffer[bytesRecibidos] = '\0';
        printf("Me llegaron %d bytes con el mensaje: %s \n", bytesRecibidos, buffer);

        // ----------------------------------------------------------

        updateClient(pl, aliens, bullets, shields,renderer,buffer);
        SDL_Delay(50);
    }
    return 0;
}

int connect_client(updateFunction updateClient,
                   eventPollFunction eventPoll,
                   drawFunction draw,
                   Player *pl, int speed,
                   struct LinkedList *aliens,
                   struct LinkedList *bullets,
                   struct LinkedList *shields,
                   SDL_Renderer *renderer) {

    int done = 0;
    int cliente = openSocket();
    if(cliente == -1) {
        perror("Error al abrir socket");
        return 1;
    }
    if(bindPortCliente(cliente,puerto) == -1)
        return 1;

    printf("Estoy escuchando\n");
    listen(cliente, 100);

    char buffer[12000];
    char len[5];
    char *resp = "ff";
    int largo;
    SDL_Texture* textClient = loadText(renderer, "Cliente");
    while (done != 1) {

        ///////////////// Recibe el largo  |||||||||||||||||||||||||||
        int bytesRecibidos = recv(cliente, len, 5, 0);

        if (bytesRecibidos <= 0) {
            perror("Pus se deconecto we cliente2");
            break;
        }

        len[bytesRecibidos] = '\0';
        printf("Me llegaron %d bytes con el mensaje: %s \n", bytesRecibidos, len);

        largo = atoi(len);
        printf("largo: %d\n",largo);
        // ----------------------------------------------------------


        ///////////////// Recibe el mensaje |||||||||||||||||||||||||
        bytesRecibidos = recv(cliente, buffer, largo, 0);
        if (bytesRecibidos <= 0) {
            perror("Pus se deconecto we cliente1");
            break;
        }
        buffer[bytesRecibidos] = '\0';
        printf("Me llegaron %d bytes con el mensaje: %s \n", bytesRecibidos, buffer);

        // ----------------------------------------------------------


        updateClient(pl, aliens, bullets, shields,renderer,buffer);
        draw(renderer, pl, aliens, bullets, shields, textClient);
        done = eventPoll(pl, 0, bullets, renderer);

        ///////////////// Crea el mensaje |||||||||||||||||||||||||

        json_object *jobj = json_object_new_object();
        playerJson(pl, jobj);
        aliensJson(aliens, jobj);
        bulletsJson(bullets, jobj);
        blocksJson(shields, jobj);
        speedJson(speed, jobj);

        // --------------------------------------------------------


        ///////////////// Envia el largo |||||||||||||||

        char const *mensaje = json_object_to_json_string(jobj);
        int length = strlen(mensaje);
        char messageLength[5];
        sprintf(messageLength, "%d", length);

        send(cliente, messageLength, strlen(messageLength), 0);
        usleep(100);
        // ---------------------------------------------


        //////////////// Envia el mensaje Json |||||||||||||||

        send(cliente,mensaje,strlen(mensaje),0);

        // ----------------------------------------------------
        SDL_Delay(50);

    }
    return 0;
}