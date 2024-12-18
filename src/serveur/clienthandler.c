#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>
#include <stdbool.h>

#include "clienthandler.h"
#include "../chat/parametres.h"


Client clients[MAX_CLIENTS];
int clientCount = 0;

//Ajoute client et son socketfd dans la liste des 1000 clients disponibles 
void addClient(int sockfd, DataClient* data){
    if (clientCount < MAX_CLIENTS){
        clients[clientCount].sockfd = sockfd;
        strncpy(clients[clientCount].pseudo, data->pseudo, sizeof(clients[clientCount].pseudo) - 1);
        clients[clientCount].isBot = data->isBot;
        clients[clientCount].isManuel = data->isManuel;
        clients[clientCount].pseudo[sizeof(clients[clientCount].pseudo) - 1] = '\0';

        clientCount++;
    }
}

//Enleve client selon son socketfd
void removeClient(int sockfd) {
    for (int i = 0; i < clientCount; i++) {
        if (clients[i].sockfd == sockfd) {
            clients[i] = clients[clientCount - 1]; 
            clientCount--;
            return;
        }
    }
}

//Trouve socketfd avec pseudo 
int getSocketfd(const char *pseudo) {
    for (int i = 0; i < clientCount; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            return clients[i].sockfd;
        }
    }
    return -1; 
}


const char* getName(int sockfd) {
    for (int i = 0; i < clientCount; i++) {
        if (clients[i].sockfd == sockfd) {
            return clients[i].pseudo; // Retourne le pseudo associé au socket
        }
    }
    return NULL; // Retourne NULL si aucun client trouvé
}

