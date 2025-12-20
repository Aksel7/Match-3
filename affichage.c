#include <stdio.h>
#include <stdlib.h>
#include "affichage.h"
#include "affichage_console.h" // C'est lui qui contient les #define BLACK, RED, etc.


const char* NOM_PIERRES[] = {"", "EMERAUDE", "RUBIS", "SAPHIR", "TOPAZE", "DIAMANT"};

const int CODE_COULEURS[] = { BLACK, LIGHTGREEN, LIGHTRED, LIGHTBLUE, YELLOW, LIGHTCYAN };

// ==========================================
// FONCTIONS D'INTERFACE (HUD & PLATEAU)
// ==========================================

void afficherInfosHUD(char pseudo[], int niveau, int coups, int vies, int objectifs[6], int progression[6], int tempsRestant) {
    text_color(WHITE); 
    printf("====================================================\n");
    printf(" JOUEUR: %s | NIV: %d | VIES: %d | COUPS: %d | TEMPS: ", pseudo, niveau, vies, coups);
    
    if (tempsRestant <= 10) text_color(LIGHTRED);
    else text_color(LIGHTGREEN);
    printf("%ds", tempsRestant);
    text_color(WHITE); 
    
    printf("\n====================================================\n");
    printf(" CONTRAT (Pierres a collecter) :\n");
    
    int a_un_objectif = 0;
    for (int i = 1; i <= 5; i++) {
        if (objectifs[i] > 0) {
            text_color(CODE_COULEURS[i]);
            printf("  %s", NOM_PIERRES[i]);
            
            text_color(WHITE);
            printf(" : %d / %d", progression[i], objectifs[i]);
            
            if (progression[i] >= objectifs[i]) {
                text_color(LIGHTGREEN);
                printf(" (OK!)");
            }
            printf("\n");
            a_un_objectif = 1;
        }
    }
    text_color(WHITE);
    if(!a_un_objectif) printf("  (Aucun objectif)\n");
    printf("----------------------------------------------------\n");
    printf(" [Z/Q/S/D] Deplacer | [ESPACE] Selection | [P] Pause\n");
}

void afficherPlateau(int grille[LIGNES][COLONNES], int cX, int cY, int selActive, int selX, int selY) {
    printf("\n");
    for (int i = 0; i < LIGNES; i++) {
        printf("   "); 
        for (int j = 0; j < COLONNES; j++) {
            
            int valeur = grille[i][j];
            int type = valeur % 10;       
            int estSpecial = (valeur > 10); 

            // Traduction de l'ID logique (1, 2...) vers la couleur Console
            int couleur = CODE_COULEURS[type];
            
            int estCurseur = (i == cX && j == cY);
            int estSelectionne = (selActive && i == selX && j == selY);

            // Gestion Curseur (Texte Blanc)
            text_color(WHITE);
            if (estSelectionne && estCurseur) printf("<");
            else if (estCurseur) printf("[");
            else if (estSelectionne) printf("*");
            else printf(" "); 

            // Affichage de la Pierre
            if (estSpecial) {
                // BOMBE : Fond coloré, texte Blanc
                text_color(WHITE); 
                printf("%d", type); 
            } 
            else {
                // NORMAL : Fond noir, texte coloré
                text_color(couleur);
                printf("%d", type);
            }

            // Reset pour la fermeture du curseur
            text_color(WHITE);
            if (estSelectionne && estCurseur) printf(">");
            else if (estCurseur) printf("]");
            else if (estSelectionne) printf("*");
            else printf(" "); 
        }
        printf("\n"); 
    }
    // On remet la couleur par défaut à la fin
    text_color(WHITE);
}

void afficherRegles() {
    clrscr();
    text_color(LIGHTCYAN);
    printf("====================================================\n");
    printf("             REGLES DU JEU : ECE HEROES             \n");
    printf("====================================================\n\n");
    text_color(WHITE);
    printf("1. PRINCIPE :\n");
    printf("   Associez au moins 3 pierres identiques pour remplir\n");
    printf("   le contrat.\n\n");
    printf("2. COMMANDES :\n");
    printf("   - Z/Q/S/D : Deplacer le curseur.\n");
    printf("   - ESPACE  : Selectionner/Permuter.\n");
    printf("   - P       : Abandonner.\n\n");
    printf("3. BONUS :\n");
    printf("   - Ligne de 5 : Cree une BOMBE (Fond colore).\n");
    printf("   - Bombe + 2 items : EXPLOSION DE ZONE (3x3).\n");
    printf("   - Ligne de 6 : Detruit toute la couleur.\n");
    printf("   - Croix (5+5) : Explosion Ligne + Colonne.\n");
    printf("   - CARRE 4x4 : Detruit le bloc entier !\n\n");
    printf("====================================================\n");
    printf("Appuyez sur une touche...");
    getch();
}
