#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "parametres.h"
#include "clienthandler.h"

#define PORT 1234
#define BUFFER_SIZE 1024

// Fonction pour recevoir les messages du serveur
void *receiveMessages(void *socket) {
    int sock = *(int *)socket;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            close(sock);
            exit(1);
        }
        printf("%s\n", buffer);
    }
    return NULL;
}

// Fonction pour envoyer des messages au serveur
void *sendMessages(void *socket) {
    int sock = *(int *)socket;
    char buffer[BUFFER_SIZE];

    while (1) {
        fgets(buffer, sizeof(buffer), stdin);

        // Retirer le saut de ligne de `fgets`
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (send(sock, buffer, strlen(buffer), 0) < 0) {
            perror("Send error");
            break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    DataClient data;
    GererParametres(argc, argv, &data);

    char *ip = "127.0.0.1";
    int sock;
    struct sockaddr_in addr;
    pthread_t recvThread, sendThread;

    // Créer le socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket error");
        exit(1);
    }

    // Préparer l'adresse du serveur
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = PORT;
    addr.sin_addr.s_addr = inet_addr(ip);

    // Se connecter au serveur
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Connect error");
        exit(1);
    }

    // Envoyer le pseudo au serveur
    send(sock, &data, sizeof(DataClient), 0);
    printf("Connected to server as %s\n", data.pseudo);

    // Créer un thread pour recevoir des messages
    if (pthread_create(&recvThread, NULL, receiveMessages, &sock) != 0) {
        perror("Thread creation error for receiving messages");
        close(sock);
        exit(1);
    }

    // Créer un thread pour envoyer des messages
    if (pthread_create(&sendThread, NULL, sendMessages, &sock) != 0) {
        perror("Thread creation error for sending messages");
        close(sock);
        exit(1);
    }

    // Attendre que les threads se terminent
    pthread_join(recvThread, NULL);
    pthread_join(sendThread, NULL);

    // Fermer le socket
    close(sock);

    return 0;
}
