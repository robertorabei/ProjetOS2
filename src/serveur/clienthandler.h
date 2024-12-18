#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>

#include "../chat/parametres.h"

#define MAX_CLIENTS 1000
#define PSEUDO_MAX_LENGTH 30

// Structure représentant un client
typedef struct {
    int sockfd;                 // Descripteur de socket du client
    char pseudo[PSEUDO_MAX_LENGTH]; // Pseudonyme du client
    bool isBot;                 
    bool isManuel;
} Client;

// Liste globale des clients et compteur
extern Client clients[MAX_CLIENTS];
extern int clientCount;

/**
 * Ajoute un client à la liste des clients connectés.
 *
 * @param sockfd Le descripteur de socket du client.
 * @param pseudo Le pseudonyme du client.
 */
void addClient(int sockfd, DataClient* data);

/**
 * Supprime un client de la liste des clients connectés.
 *
 * @param sockfd Le descripteur de socket du client à supprimer.
 */
void removeClient(int sockfd);

/**
 * Trouve le descripteur de socket d'un client à partir de son pseudonyme.
 *
 * @param pseudo Le pseudonyme du client à chercher.
 * @return Le descripteur de socket du client, ou -1 si introuvable.
 */
int getSocketfd(const char *pseudo);


const char* getName(int sockfd);

#endif // CLIENTHANDLER_H
