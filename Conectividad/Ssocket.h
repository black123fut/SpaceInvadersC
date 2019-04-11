
#ifndef SPACEINVADERSC_MASTER_SSOCKET_H
#define SPACEINVADERSC_MASTER_SSOCKET_H

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int openSocket();
int bindPortServidor(int socket, int port);
int bindPortCliente(int socket, int port);

#endif //SPACEINVADERSC_MASTER_SSOCKET_H
