#ifndef SERVER_H
#define SERVER_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <ctype.h>

#include "clienthandler.h"

#define PORT 1234        // Port utilisé par le serveur
#define BUFFER_SIZE 1024 // Taille du tampon pour les messages

/**
 * Extrait le pseudonyme du destinataire d'un message.
 *
 * @param buffer Le message reçu contenant le pseudonyme et le texte.
 * @return Une chaîne allouée dynamiquement contenant le pseudonyme du destinataire,
 *         ou NULL si aucun pseudonyme n'est trouvé. Le code appelant doit libérer la mémoire.
 */
char* getDestinataire(char* buffer);

/**
 * Gère les messages reçus d'un client.
 *
 * @param client_sock Le descripteur de socket du client.
 * @param readfds Le set des descripteurs surveillés (fd_set).
 */
void handle_client_message(int client_sock, fd_set *readfds);

/**
 * Accepte une nouvelle connexion client et l'ajoute à la liste des clients connectés.
 *
 * @param server_sock Le descripteur de socket du serveur.
 * @param client_addr La structure contenant l'adresse du client.
 */
void newConnection(int server_sock, struct sockaddr_in *client_addr);

#endif // SERVER_H

