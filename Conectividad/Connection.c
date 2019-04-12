#include "Connection.h"

//struct server_functions *ServerFunc;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//struct LinkedList *clients;
//int n = 0;
//
////void sendtoall(char const *message, int curr) {
////    pthread_mutex_lock(&mutex);
////
////    for (int i = 0; i < length(&clients); ++i) {
////        int client = *(int *) get(&clients, i);
////
////        if (send(client, message, strlen(message), 0) < 0) {
////            perror("Error al enviar");
////            continue;
////        }
////
////    }
////    pthread_mutex_unlock(&mutex);
////}
//
//void *sendtoall(void *sock) {
//    struct client_info client = *((struct client_info *) sock);
//
//    updateFunction updateServer = ServerFunc->updateServer;
//    updateFunction updateClient = ServerFunc->updateClient;
//    eventPollFunction eventPoll = ServerFunc->eventPoll;
//    drawFunction draw = ServerFunc->draw;
//    Player *pl = ServerFunc->pl;
//    int speed = ServerFunc->speed;
//    struct LinkedList *aliens = ServerFunc->aliens;
//    struct LinkedList *bullets = ServerFunc->bullets;
//    struct LinkedList *shields = ServerFunc->shields;
//    SDL_Renderer *renderer = ServerFunc->renderer;
//    char buffer[12000];
//    char len[5];
//    int largo;
//
//    while (1) {
//        printf("\nhola");
//        updateServer(pl, aliens, bullets, shields, renderer, NULL);
//        //draw(renderer, pl, aliens, bullets, shields, NULL);
//        //done = eventPoll(pl, 0, bullets, renderer);
//
//        ///////////////// Crea el mensaje |||||||||||||||||||||||||
//
//        json_object *jobj = json_object_new_object();
//        playerJson(pl, jobj);
//        aliensJson(aliens, jobj);
//        bulletsJson(bullets, jobj);
//        blocksJson(shields, jobj);
//        speedJson(speed, jobj);
//
//        // --------------------------------------------------------
//
//        char const *mensaje = json_object_to_json_string(jobj);
//
//        ///////////////// Envia el largo |||||||||||||||||||||
//        int length = strlen(mensaje);
//        printf("\nlargo de de de %i \n", length);
//        char messageLength[5];
//        sprintf(messageLength, "%d", length);
//
//        printf("\nlargo de men: %lu\n", strlen(messageLength));
//
//
//        if (send(client.sockno, messageLength, strlen(messageLength), 0) < 0) {
//            perror("Error al enviar1");
//            continue;
//        }
//
//
////        send(client.sockno, messageLength, strlen(messageLength), 0);
//        usleep(80);
//        // ---------------------------------------------------
//
//
//        ///////////////// Envia el mensaje Json |||||||||||||||
//
//        if (send(client.sockno, mensaje, strlen(mensaje), 0) < 0) {
//            perror("Error al enviar2");
//            continue;
//        }
//
////        send(client.sockno,mensaje,strlen(mensaje),0);
//
//        // ----------------------------------------------------
//
//        ///////////////// Recibe el largo |||||||||||||||||||||
//        int bytesRecibidos = recv(client.sockno, len, 5, 0);
//
//        if (bytesRecibidos < 0) {
//            perror("Pus se deconecto we server");
//        }
//
//        len[bytesRecibidos] = '\0';
//        if (strcmp(len, "quit") > 0) {
//            printf("ESC pressed!");
//            break;
//        }
//        //printf("Me llegaron %d bytes con el mensaje: %s \n", bytesRecibidos, len);
//
//        largo = atoi(len);
//        printf("largo: %d\n",largo);
//        // ---------------------------------------------------
//
//
//        ///////////////// Recibe el mensaje |||||||||||||||||||||||||
//        bytesRecibidos = recv(client.sockno, buffer, largo, 0);
//        if (bytesRecibidos < 0) {
//            perror("Pus se deconecto we cliente1");
//            break;
//        }
//        buffer[bytesRecibidos] = '\0';
//        if (strcmp(len, "quit") > 0) {
//            printf("ESC pressed!");
//            break;
//        }
//        printf("Me llegaron %d bytes con el mensaje: %s \n", bytesRecibidos, buffer);
//
//        // ----------------------------------------------------------
//
//        updateClient(pl, aliens, bullets, shields,renderer,buffer);
//        SDL_Delay(50);
//    }
//
//    pthread_mutex_lock(&mutex);
//    printf("%s disconnected\n", client.ip);
//    for (int i = 0; i < length(clients); ++i) {
//        int tmp = *(int *) get(clients, i);
//
//        if (tmp == client.sockno) {
//            delete_node(clients, i, "");
//        }
//    }
//    pthread_mutex_unlock(&mutex);
//}
//
////void *runGame() {
////    updateFunction updateServer = ServerFunc.updateServer;
////    updateFunction updateClient = ServerFunc.updateClient;
////    eventPollFunction eventPoll = ServerFunc.eventPoll;
////    drawFunction draw = ServerFunc.draw;
////    Player *pl = ServerFunc.pl;
////    int speed = ServerFunc.speed;
////
////    struct LinkedList *aliens = ServerFunc.aliens;
////    struct LinkedList *bullets = ServerFunc.bullets;
////    struct LinkedList *shields = ServerFunc.shields;
////    SDL_Renderer *renderer = ServerFunc.renderer;
////    SDL_Texture* textServer = loadText(renderer, "Servidor");
////
////    while(1) {
////        updateServer(&pl, aliens, bullets, shields, renderer, textServer);
////        draw(renderer, &pl, aliens, bullets, shields, textServer);
////        //eventPoll(&pl, 0, bullets, renderer);
////        SDL_Delay(50);
////    }
////}
//
//
//int connect_server(updateFunction updateServer,
//                   updateFunction updateClient,
//                    eventPollFunction eventPoll,
//                    drawFunction draw,
//                    Player *pl, int speed,
//                    struct LinkedList *aliens,
//                    struct LinkedList *bullets,
//                    struct LinkedList *shields,
//                    SDL_Renderer *renderer) {
//
////    ServerFunc->updateServer = updateServer;
////    ServerFunc->updateClient = updateClient;
////    ServerFunc->eventPoll = eventPoll ;
////    ServerFunc->draw = draw;
////    ServerFunc->pl = pl;
////    ServerFunc->speed = speed;
//
//
//    struct sockaddr_in my_addr,their_addr;
//    int my_sock;
//    int their_sock;
//    socklen_t their_addr_size;
//    pthread_t sendt,play;
//    struct client_info cl;
//
//    //truepthread_create(&play,NULL,runGame,&cl);
//    clients = (struct LinkedList *) malloc(sizeof(struct LinkedList));
//    createList(clients, sizeof(int), NULL);
//
//    char ip[INET_ADDRSTRLEN];
//
//    my_sock = openSocket();
//    memset(my_addr.sin_zero,'\0',sizeof(my_addr.sin_zero));
//    my_addr.sin_family = AF_INET;
//    my_addr.sin_port = htons(puerto);
//    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//    their_addr_size = sizeof(their_addr);
//
//    if(bind(my_sock,(struct sockaddr *)&my_addr,sizeof(my_addr)) != 0) {
//        perror("binding unsuccessful");
//        exit(1);
//    }
//
//    if(listen(my_sock,5) != 0) {
//        perror("listening unsuccessful");
//        exit(1);
//    }
//
//    while(1) {
//        printf("HOLO HOLO HOLO");
//        if((their_sock = accept(my_sock,(struct sockaddr *)&their_addr,&their_addr_size)) < 0) {
//            perror("accept unsuccessful");
//            exit(1);
//        }
//        pthread_mutex_lock(&mutex);
//        inet_ntop(AF_INET, (struct sockaddr *)&their_addr, ip, INET_ADDRSTRLEN);
//        printf("%s connected\n",ip);
//        cl.sockno = their_sock;
//        strcpy(cl.ip,ip);
//        add(clients, &their_sock);
//        n++;
//        pthread_create(&sendt,NULL,sendtoall,&cl);
//        pthread_mutex_unlock(&mutex);
//    }
////add(&clients, &new_client);
//    //-------------------------------------------------------
//
////    struct sockaddr_in direccionCliente;
////    unsigned int tamanoDireccion;
////    int cliente = accept(servidor, (void *) &direccionCliente, &tamanoDireccion);
//
////    printf("Recibi una conexion en %d!!\n", cliente);
//
//    //---------------------------------------------------
////            usleep(100);
//
//
//    return 0;
//}

int connect_client_player(updateFunction updateClient,
                          eventPollFunction eventPoll,
                          drawFunction draw,
                          Player *pl, int speed,
                          struct LinkedList *aliens,
                          struct LinkedList *bullets,
                          struct LinkedList *shields,
                          SDL_Renderer *renderer, int isObserver) {
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

    SDL_Texture* textClient;
    if (isObserver)
        textClient = loadText(renderer, "Cliente-Observador");
    else
        textClient = loadText(renderer, "Cliente-Jugador");

    while (1) {
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

        // ----------------------------------------------------------

        updateClient(pl, aliens, bullets, shields,renderer,buffer);
        draw(renderer, pl, aliens, bullets, shields, textClient);

        if (!isObserver)
            done = eventPoll(pl, 0, bullets, renderer);

        ///////////////// Crea el mensaje |||||||||||||||||||||||||

        json_object *jobj = json_object_new_object();
        if (isObserver)
            typeClient(1, jobj);
        else
            typeClient(0, jobj);

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
        if (done != 1)
            sprintf(messageLength, "%d", length);
        else
            strcat(messageLength, "quit");

        //printf("\nNesasf size: %lu\n", strlen(messageLength));
        int response = send(cliente, messageLength, strlen(messageLength), 0);
        if (response < 0) {
            perror("message not sent1");
            exit(1);
        }
        printf("A server length: %s\n", messageLength);
        usleep(80);
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

        //printf("A server: %s", mensaje);
        // ----------------------------------------------------
        SDL_Delay(50);

    }
    return 0;
}