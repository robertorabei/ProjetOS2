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
   bool isBot;
   bool isManuel;
} OptionsProgramme;


void GererParameteres(int argc, char* argv[], OptionsProgramme* options);

#endif
