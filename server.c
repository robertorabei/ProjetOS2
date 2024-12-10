#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <ctype.h>

#include "clienthandler.h"

#define PORT 1234
#define BUFFER_SIZE 1024

// Fonction temporaire qui a pour but de prendre le buffer reçu en extraire le pseudo du destinataire en début de message et de le renvoyer sans le pseudo dans le buffer au destinataire
char* getDestinataire(const char* buffer) {
    while (*buffer && isspace((unsigned char)*buffer)) { // Enlever les espaces initiaux
        buffer++;
    }
    const char *space = strchr(buffer, ' '); // Trouver le premier espace
    if (space) {
        size_t pseudo_length = space - buffer;
        char *pseudo = malloc(pseudo_length + 1);
        if (!pseudo) {
            perror("Memory allocation failed");
            exit(1);
        }
        strncpy(pseudo, buffer, pseudo_length);
        pseudo[pseudo_length] = '\0';
        printf("Destinataire: %s\n", pseudo);
        return pseudo; // Pas de free ici, le code appelant doit gérer la mémoire
    }
    return NULL;
}

//Cette fonction ne va pas durer elle sert à vérifier que les messages arrivent bel et bien dans le serveur
void handle_client_message(int client_sock, fd_set *readfds) {
    char buffer[BUFFER_SIZE];
    int n = recv(client_sock, buffer, sizeof(buffer) - 1, 0);

    if (n <= 0) {
        // Gestion de la déconnexion
        if (n == 0) {
            printf("Client %d disconnected.\n", client_sock);
        } else {
            perror("Recv error");
        }
        close(client_sock);
        FD_CLR(client_sock, readfds);
        removeClient(client_sock); // Assurez-vous d'avoir cette fonction
    } else {
        buffer[n] = '\0';
        char* destinataire = getDestinataire(buffer);
        if (destinataire) {
            int dest_sock = getSocketfd(destinataire); // Implémentez cette fonction
            if (dest_sock > 0) {
                send(dest_sock, buffer, strlen(buffer), 0);
            } else {
                printf("Cette personne (%s) n'est pas connectée.\n", destinataire); //normalement c'est un stderr je change bientôt
            }
            free(destinataire);
        }
    }
}


void newConnection(int server_sock, struct sockaddr_in *client_addr) {
    socklen_t addr_size = sizeof(*client_addr); // Taille de l'adresse client
    int new_sock = accept(server_sock, (struct sockaddr *)client_addr, &addr_size);
    if (new_sock < 0) {
        perror("Accept error");
        exit(1);
    }
    // Recevoir le pseudo du client
    char pseudo[PSEUDO_MAX_LENGTH];
    int n = recv(new_sock, pseudo, sizeof(pseudo), 0); // Dès que client se connecte il envoye son pseudo au serveur, ici le serveur s'occupe de le réceptionner 

    // Ajouter le client à la liste des clients connectés
    addClient(new_sock, pseudo);
    printf("[+] %s connected to chat.\n", pseudo);
}

int main() {
    int server_sock, max_sd, activity;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    fd_set readfds; // Ensemble de fichiers à surveiller

    
    // Créer le socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket error");
        exit(1);
    }

    // Préparer l'adresse du serveur
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Lier le socket à l'adresse
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind error");
        exit(1);
    }

    // Écouter les connexions entrantes
    if (listen(server_sock, 5) < 0) { // 5 parce que la longueur de la file d’attente pour les nouvelles connexions entrantes est de 5
        perror("Listen error");
        exit(1);
    }

    printf("[+]Server listening on port %d...\n", PORT);

    // Boucle principale du serveur
    while (1) {
        FD_ZERO(&readfds); // Réinitialiser le tableau readfds
        FD_SET(server_sock, &readfds); // Ajouter le socket serveur à readfds
        max_sd = server_sock;

        // Ajouter les sockets des clients à readfds
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = clients[i].sockfd;
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        // Utiliser select pour attendre l'activité sur l'un des sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("Select error");
            exit(1);
        }

        // Si le serveur reçoit une nouvelle connexion
        if (FD_ISSET(server_sock, &readfds)) {
            newConnection(server_sock, &client_addr);
        }

        // Gérer les messages des clients
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = clients[i].sockfd;
            if (FD_ISSET(sd, &readfds)) {
                handle_client_message(sd, &readfds);
            }
        }
    }

    return 0;
}
