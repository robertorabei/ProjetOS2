#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>

#include "clienthandler.h"
#include "../chat/parametres.h"


Client clients[MAX_CLIENTS];
int clientCount = 0;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

//Ajoute client et son socketfd dans la liste des 1000 clients disponibles 
void addClient(int sockfd, DataClient* data){
	pthread_mutex_lock(&clients_mutex);
    if (clientCount < MAX_CLIENTS){
        clients[clientCount].sockfd = sockfd;
        strncpy(clients[clientCount].pseudo, data->pseudo, sizeof(clients[clientCount].pseudo) - 1);
        clients[clientCount].isBot = data->isBot;
        clients[clientCount].isManuel = data->isManuel;
        clients[clientCount].pseudo[sizeof(clients[clientCount].pseudo) - 1] = '\0';

        clientCount++;
    }
    pthread_mutex_unlock(&clients_mutex);
}

//Enleve client selon son socketfd
void removeClient(int sockfd) {
	pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < clientCount; i++) {
        if (clients[i].sockfd == sockfd) {
            clients[i] = clients[clientCount - 1]; 
            clientCount--;
            return;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

//Trouve socketfd avec pseudo 
int getSocketfd(const char *pseudo) {
	pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < clientCount; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
        	pthread_mutex_unlock(&clients_mutex);
            return clients[i].sockfd;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    return -1; 
}


const char* getName(int sockfd) {
	pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < clientCount; i++) {
        if (clients[i].sockfd == sockfd) {
        	pthread_mutex_unlock(&clients_mutex);
            return clients[i].pseudo; // Retourne le pseudo associé au socket
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    return NULL; // Retourne NULL si aucun client trouvé
}

