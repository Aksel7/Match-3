#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <stdio.h>

/* Structure pour les informations du niveau */
typedef struct {
    int vies;
    int coups;
    int temps;
    int objectifs[5];
    int score;
} InfosNiveau;

/* Affiche le menu principal */
void afficherMenu(void);

/* Affiche le plateau 9x9 */
void afficherPlateau(int plateau[9][9], int curseurX, int curseurY, int selectionActive);

/* Affiche les informations du niveau */
void afficherInfosNiveau(int coupsRestants, int score, int objectifs[5], char* nomsPierres[5]);

/* Lit une touche clavier (z q s d espace p) */
char lireInputClavier(void);

/* Efface l’écran (Windows) */
void effacerEcran(void);

#endif
