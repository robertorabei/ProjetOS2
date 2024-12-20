#include <signal.h>
#include <stdio.h>
#include <stdbool.h>

#include "signal.h"
#include "buffer.h"


static volatile sig_atomic_t receptionSIGINT = 0; 

// Gestionnaire SIGINT
static void HandlerSigint(int sig) {
    if (sig == SIGINT) {
        receptionSIGINT = 1; 
        printf("\nSIGINT reçu.\n");
    }
}

// Réinitialiser l'état de SIGINT
void ResetSigint(void) {
    receptionSIGINT = 0;
}

// Verififcation de l'attente de SIGINT
bool LoadingSigint(void) {
    return receptionSIGINT;
}

// Configuration des gestionnaires de signaux
bool SignalsConfiguration(void) {
    struct sigaction action;
   
    action.sa_handler = HandlerSigint;
    sigemptyset(&action.sa_mask); // Initialiser le masque
    action.sa_flags = 0;

    if (sigaction(SIGINT, &action, NULL) < 0) {
        perror("La configuration des signaux a échoué.");
        return false;
    }
    // Ignorer SIGPIPE
    signal(SIGPIPE,SIG_IGN);

    return true;
}

// Traitement de SIGINT
bool TraiterSigint(SuspendedMessages* suspendedMessages) {
    if (suspendedMessages != NULL && LoadingSigint()) {
        // SIGINT detecté et gestion des messages en attente
        FlushBuffer(suspendedMessages);
        ResetSigint();

        return true;
    }

    return false;
}