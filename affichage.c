
#include <stdio.h>
#include "affichage.h"

void afficherInfosNiveau(int coups, int score, int temps, int vies, int objectif[5])
{
    printf("\n==============================\n");
    printf("     INFORMATIONS NIVEAU\n");
    printf("==============================\n");

    printf("Vies restantes  : %d\n", vies);
    printf("Coups restants  : %d\n", coups);
    printf("Temps restant   : %d secondes\n", temps);
    printf("Score           : %d\n", score);

    printf("\n--- OBJECTIFS ---\n");
    printf("Emeraude     : %d\n", objectif[0]);
    printf("Saphir     : %d\n", objectif[1]);
    printf("Rubis  : %d\n", objectif[2]);
    printf("Diamant      : %d\n", objectif[3]);
    printf("Topaze     : %d\n", objectif[4]);

    printf("==============================\n");
}
