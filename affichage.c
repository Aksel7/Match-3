#include "affichage.h"
#include <stdlib.h>
#include <conio.h>   

// Codes couleurs ANSI
#define RESET   "\x1b[0m"
#define ROUGE   "\x1b[31m"
#define VERT    "\x1b[32m"
#define BLEU    "\x1b[34m"
#define JAUNE   "\x1b[33m"
#define BLANC   "\x1b[37m"

/* Efface l'écran */
void effacerEcran(void) {
    system("cls");
}

/* Menu principal */
void afficherMenu(void) {
    printf("=====================================\n");
    printf("        ECE HEROES - Menu             \n");
    printf("=====================================\n\n");
    printf("z : haut | s : bas | q : gauche | d : droite\n");
    printf("espace : selection | p : quitter\n\n");
}

/* Affichage du plateau */
void afficherPlateau(int plateau[9][9], int curseurX, int curseurY, int selectionActive) {
    printf("\n=== Plateau de jeu ===\n\n");

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            char* couleur;
            switch (plateau[i][j]) {
                case 0: couleur = VERT; break;
                case 1: couleur = BLEU; break;
                case 2: couleur = ROUGE; break;
                case 3: couleur = BLANC; break;
                case 4: couleur = JAUNE; break;
                default: couleur = RESET; break;
            }

            if (i == curseurX && j == curseurY) {
                if (selectionActive)
                    printf("<%s%d%s> ", couleur, plateau[i][j], RESET);
                else
                    printf("[%s%d%s] ", couleur, plateau[i][j], RESET);
            } else {
                printf(" %s%d%s  ", couleur, plateau[i][j], RESET);
            }
        }
        printf("\n\n");
    }
}

/* Informations du niveau */
void afficherInfosNiveau(int coupsRestants, int score, int objectifs[5], char* nomsPierres[5]) {
    printf("=== Informations du Niveau ===\n");
    printf("Coups restants : %d\n", coupsRestants);
    printf("Score : %d\n", score);
    printf("Objectifs :\n");

    for (int i = 0; i < 5; i++) {
        printf(" - %s : %d\n", nomsPierres[i], objectifs[i]);
    }

    printf("==============================\n\n");
}

/* Lecture clavier */
char lireInputClavier(void) {
    char touche = _getch();  // lecture directe (Windows)

    if (touche == 'z' || touche == 'q' || touche == 's' ||
        touche == 'd' || touche == ' ' || touche == 'p') {
        return touche;
    }

    return 0;
}



int main() {
    int plateau[9][9] = {
        {0,1,2,3,4,0,1,2,3},
        {1,2,3,4,0,1,2,3,4},
        {2,3,4,0,1,2,3,4,0},
        {3,4,0,1,2,3,4,0,1},
        {4,0,1,2,3,4,0,1,2},
        {0,1,2,3,4,0,1,2,3},
        {1,2,3,4,0,1,2,3,4},
        {2,3,4,0,1,2,3,4,0},
        {3,4,0,1,2,3,4,0,1}
    };

    int x = 0;
    int y = 0;
    int selection = 0;

    int coupsRestants = 15;
    int score = 1200;
    int objectifs[5] = {3, 4, 5, 2, 1};

    char* nomsPierres[5] = {
        "Emeraude",
        "Saphir",
        "Rubis",
        "Diamant",
        "Topaze"
    };

    char touche;

    afficherMenu();

    // Boucle de test
    while (1) {
        effacerEcran();

        afficherPlateau(plateau, x, y, selection);
        afficherInfosNiveau(coupsRestants, score, objectifs, nomsPierres);

        touche = lireInputClavier();

        if (touche == 'p') {
            break; // quitter le test
        }

        switch (touche) {
            case 'z': if (x > 0) x--; break;
            case 's': if (x < 8) x++; break;
            case 'q': if (y > 0) y--; break;
            case 'd': if (y < 8) y++; break;
            case ' ': selection = !selection; break;
            default: break;
        }
    }

    printf("Fin du test clavier.\n");
    return 0;
}
