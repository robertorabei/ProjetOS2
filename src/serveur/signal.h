#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdbool.h>

/**
 * Struct représentant la configuration d'un gestionnaire de signaux.
 * Elle est utilisée par la fonction sigaction pour configurer la gestion
 * des signaux.
 */
struct sigaction{
    // Ensemble de drapeaux controlant la manière de gérer les signaux
    int sa_flags;


    // Ensemble de signaux qui sont masqués pendant l'exécution du gestionnaire de signaux.
    sigset_t sa_mask;

    /**
     * Sa_handler : un pointeur vers la fonction qui sera exécutée lorsqu'un
     * signal est reçu. Dans ce cas, la fonction doit prendre un argument de
     * type int (le numéro du signal) et ne rien retourner (void).
     */
    // Pointeur vers la fonction qui sera executé à la réception d'un signal
    void (*sa_handler)(int);
};


/**
 * Réinitialise l'état de réception de SIGINT.
 */
void ResetSigint(void);

/**
 * Vérifie si SIGINT est en attente.
 *
 * @return true si SIGINT a été reçu, false sinon.
 */
bool LoadingSigint(void);

/**
 * Configure les gestionnaires de signaux.
 *
 * SIGINT : gestion active via `HandlerSigint`.
 * SIGPIPE : ignoré 
 *  *
 * @return true si la configuration réussit, false sinon.
 */
bool SignalsConfiguration(void);

#endif //SIGNAL_H