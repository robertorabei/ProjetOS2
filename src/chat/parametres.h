#ifndef PARAMETRES_H__
#define PARAMETRES_H__

#include <stdbool.h>

#define MAX_PSEUDO_DESTINATAIRE 30

enum {
   CODE_RETOUR_NORMAL,
   CODE_RETOUR_PARAMETRES_MANQUANTS,
   CODE_RETOUR_PSEUDO_TROP_LONG,
   CODE_RETOUR_PSEUDO_CARACTERES_INVALIDES,
   CODE_RETOUR_ARRET_SIGINT,
   CODE_RETOUR_ERREUR_AUTRE
};

typedef struct {
   char pseudo[MAX_PSEUDO_DESTINATAIRE];
   bool isBot;
   bool isManuel;
} DataClient;

/**
 * Gère les paramètres de la ligne de commande et initialise les données du client.
 *
 * @param argc 
 * @param argv 
 * @param data Pointeur vers la structure `DataClient` à remplir.
 */
void GererParametres(int argc, char* argv[], DataClient* data);

#endif
