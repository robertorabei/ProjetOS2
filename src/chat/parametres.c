#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "parametres.h"

static void VerifPseudo(int argc, char *argv[]) {
    if (argc < 2) {
        fputs("Usage: chat pseudo_utilisateur [--bot] [--manuel]\n", stderr);
        exit(CODE_RETOUR_PARAMETRES_MANQUANTS);
    }

    if (strlen(argv[1]) > MAX_PSEUDO_DESTINATAIRE) {
        fputs("Pseudo doit avoir moins de 30 caractères.\n", stderr);
        exit(CODE_RETOUR_PSEUDO_TROP_LONG);
    }

    const char caracteresInterdits[] = { '/', '-', '[', ']' };

    for (unsigned int j = 0; j < sizeof(caracteresInterdits) / sizeof(*caracteresInterdits); ++j) {
        if (strchr(argv[1], caracteresInterdits[j]) != NULL) {
            fprintf(stderr, "Le caractère '%c' n'est pas autorisé dans un pseudonyme.\n", caracteresInterdits[j]);
            exit(CODE_RETOUR_PSEUDO_CARACTERES_INVALIDES);
        }
    }

    if (strcmp(argv[1], ".") == 0 || strcmp(argv[1], "..") == 0) {
        fprintf(stderr, "Le pseudonyme '%s' n'est pas autorisé.\n", argv[1]);
        exit(CODE_RETOUR_PSEUDO_CARACTERES_INVALIDES);
    }
}


static void ExtraireData(int argc, char* argv[], DataClient* data) {
   strncpy(data->pseudo, argv[1], MAX_PSEUDO_DESTINATAIRE - 1);
   data->pseudo[MAX_PSEUDO_DESTINATAIRE - 1] = '\0';
   data->isBot = false;
   data->isManuel = false;
   for (int i = 2; i < argc; ++i) {
       if (strcmp(argv[i], "--bot") == 0) {
           data->isBot = true;
       }
       if (strcmp(argv[i], "--manuel") == 0) {
           data->isManuel = true;
       }
       if (strcmp(argv[i], "--bot") != 0 && strcmp(argv[i], "--manuel") != 0) {
           // Argument pas valide
           fprintf(stderr, "chat pseudo_utilisateur [--bot] [--manuel]\n");
           return 1;
        }
   }
}

void GererParametres(int argc, char* argv[], DataClient* data) {
   VerifPseudo(argc, argv);
   ExtraireData(argc, argv, data);
}