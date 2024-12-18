#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "parametres.h"
#include "../serveur/clienthandler.h"  

#define DEFAULT_PORT 1234
#define DEFAULT_IP "127.0.0.1"
#define BUFFER_SIZE 1024

//Initialisation statique des mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;


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

// Fonction pour vérifier l'adresse IP
bool verificationIP(const char *varEnvi,char **ip){
    char *IP_SERVEUR = getenv(varEnvi);

    if(IP_SERVEUR != NULL){
        bool isValid = inet_pton(AF_INET,IP_SERVEUR, NULL);
        if(isValid){
            *ip = IP_SERVEUR;
            return true;
        }
        // Adresse IP par défaut
        return false;
    }

}

//Fonction pour vérififer le port
bool verificationPORT(const char *varEnvi, char *port){
    char *PORT_SERVEUR = getenv(varEnvi);

    if(PORT_SERVEUR != NULL){
        const int decimalBase = 10;
        char *conversion_end;
        long numPort = strtol(PORT_SERVEUR, &conversion_end, decimalBase);

        if(*conversion_end == "\0" && numPort >= 1 && numPort <= 65535){
            *port = numPort;
            return true;
        }
    }
    //Port par défaut
    return false;
}


int main(int argc, char *argv[]) {
    DataClient data;
    GererParametres(argc, argv, &data);

    bool modeBot = data.isBot;
    bool modeManuel = data.isManuel;

    char *ip = NULL;  
    int port;
    int sock;
    struct sockaddr_in addr;
    pthread_t recvThread, sendThread;

    //Assignation de la valeur de l'adresse IP
    if(!verificationIP("IP_SERVEUR", &ip)){
       ip = DEFAULT_IP;
       printf("Utilisation de l'adresse IP par défaut : %s\n", ip);
    } else{
        printf("Adresse IP validé : %s\n", ip);
    }

    //Assignation de la valeur du port
    if(!verificationPORT("PORT_SERVEUR", &port)){
        int port = DEFAULT_PORT;
        printf("Utilisation du port par défaut %d\n", port);
    } else{
        printf("Port validé : %d\n", port);
    }

    // Créer le socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket error");
        exit(1);
    }

    // Préparer l'adresse du serveur
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = DEFAULT_PORT;
    addr.sin_addr.s_addr = inet_addr(ip);

    // Se connecter au serveur
    if (connect(sock, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
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