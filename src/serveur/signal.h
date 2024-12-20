#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdbool.h>

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
