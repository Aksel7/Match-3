#include <stdio.h>
#include "partie.h"

#define VIES_INIT 3

// Fonction qui simule un niveau
void jouerNiveau(int niveau, int *vies) {
    printf("\n==============================\n");
    printf("          NIVEAU %d\n", niveau);
    printf("==============================\n");

    printf("Simulation du jeu du niveau %d...\n", niveau);
    printf("Vous perdez une vie pour tester la mecanique.\n\n");

    (*vies)--;

    if (*vies > 0) {
        printf("Ben jouer ! Vous passez au niveau suivant.\n");
        printf("Vies restantes : %d\n", *vies);
    } else {
        printf("Vous n'avez plus de vies. Partie terminee.\n");
    }

    printf("\n------------------------------------\n");
}

// ICI Y'a le Boucle principale du jeu
void jouerPartie() {
    int niveau = 1;
    int vies = VIES_INIT;

    printf("\n=== Demarrage d'une nouvelle partie ===\n");

    while (vies > 0) {
        jouerNiveau(niveau, &vies);
        niveau++;
    }

    printf("\n=== Fin de la partie ===\n");
}

