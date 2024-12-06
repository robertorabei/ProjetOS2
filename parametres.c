#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "parametres.h"

static void VerifPseudo(int argc, char *argv[]){
    if (argc < 2){
        fputs("chat pseudo_utilisateur [--bot] [--manuel]\n", stderr);
        exit(CODE_RETOUR_PARAMETRES_MANQUANTS);
    }

    if (strlen(argv[1]) > LONGUEUR_PSEUDO_MAX){
        fputs("Pseudo doit avoir moins de 30 caractères.\n", stderr);
        exit(CODE_RETOUR_PSEUDO_TROP_LONG);
    }

    const char caracteresInterdits[] = { '/', '-', '[', ']' };

    for (int i = 1; i < 3; ++i) {
      for (unsigned int j = 0; j < sizeof(caracteresInterdits) / sizeof(*caracteresInterdits); ++j) {
         if (strchr(argv[i], caracteresInterdits[j]) != NULL) {
            fprintf(stderr, "Le caractère '%c' n'est pas autorisé dans un pseudonyme.\n", caracteresInterdits[j]);
            exit(CODE_RETOUR_PSEUDO_CARACTERES_INVALIDES);
         }
      }

      if (strcmp(argv[i], ".") == 0 || strcmp(argv[i], "..") == 0) {
         fprintf(stderr, "Le pseudonyme '%s' n'est pas autorisé.\n", argv[i]);
         exit(CODE_RETOUR_PSEUDO_CARACTERES_INVALIDES);
      }
   }
}

static void ExtraireFlags(int argc, char* argv[], OptionsProgramme* options) {
   options->isBot = false;
   options->isManuel = false;

   for (int i = 3; i < argc; ++i) {
      if (strcmp(argv[i], "--bot") == 0) {
         options->isBot = true;
      } else if (strcmp(argv[i], "--manuel") == 0) {
         options->isManuel = true;
      }
   }
}

void GererParameteres(int argc, char* argv[], OptionsProgramme* options) {
    VerifPseudo(argc, argv);
    ExtraireFlags(argc, argv, options);
}