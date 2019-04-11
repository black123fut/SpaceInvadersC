
#include "Ssocket.h"

int openSocket() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    return s;
}

int bindPortServidor(int socket, int port) {
    struct sockaddr_in dirServidor;
    dirServidor.sin_family = AF_INET;
    dirServidor.sin_addr.s_addr = INADDR_ANY;
    dirServidor.sin_port = htons(port);

    if(bind(socket, (void*)&dirServidor, sizeof(dirServidor)) != 0){
        perror("Fallo el bind");
        return -1;
    }else{
        return 1;
    }
}

int bindPortCliente(int socket, int port){
    struct sockaddr_in dirServidor;
    dirServidor.sin_family = AF_INET;
    dirServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    dirServidor.sin_port = htons(port);

    if (connect(socket, (void *) &dirServidor, sizeof(dirServidor)) != 0) {
        perror("No se pudo conectar con el servidor");
        return -1;
    }else{
        return 1;
    }
}