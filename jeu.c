#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeu.h"

// init grille
void creation_grille(int grille[LIGNES][COLONNES]) {
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            // rand 1 a 5
            grille[i][j] = (rand() % 5) + 1;
        }
    }
}

// remplit les trous
void modif_grille(int grille[LIGNES][COLONNES]) {
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if (grille[i][j] == 0) {
                grille[i][j] = (rand() % 5) + 1;
            }
        }
    }
}

// fait tomber les pieces
void gravite(int grille[LIGNES][COLONNES]) {
    for (int j = 0; j < COLONNES; j++) {
        int k = LIGNES - 1; 
        for (int i = LIGNES - 1; i >= 0; i--) {
            if (grille[i][j] != 0) {
                int temp = grille[i][j];
                grille[i][j] = 0;
                grille[k][j] = temp;
                k--;
            }
        }
    }
}

// effets speciaux

// boom zone 3x3
void effet_bombe(int grille[LIGNES][COLONNES], int progression[6], int cX, int cY) {
    int rayon = 1; 
    for (int i = cX - rayon; i <= cX + rayon; i++) {
        for (int j = cY - rayon; j <= cY + rayon; j++) {
            if (i >= 0 && i < LIGNES && j >= 0 && j < COLONNES) {
                if (grille[i][j] != 0) {
                    int type = grille[i][j] % 10;
                    progression[type]++; 
                    grille[i][j] = 0;    
                }
            }
        }
    }
}

// boom croix
void effet_croix_explosion(int grille[LIGNES][COLONNES], int progression[6], int cX, int cY) {
    // ligne
    for (int j = 0; j < COLONNES; j++) { 
        if (grille[cX][j] != 0) {
            int coul = grille[cX][j] % 10;
            grille[cX][j] = 0; progression[coul]++;
        }
    }
    // colonne
    for (int i = 0; i < LIGNES; i++) { 
        if (grille[i][cY] != 0) {
            int coul = grille[i][cY] % 10;
            grille[i][cY] = 0; progression[coul]++;
        }
    }
}

// vire toute une couleur
void effet_supprimer_couleur(int grille[LIGNES][COLONNES], int progression[6], int typePierre) {
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if ((grille[i][j] % 10) == typePierre) { 
                grille[i][j] = 0; progression[typePierre]++;
            }
        }
    }
}

// algo principal
int resoudre_plateau(int grille[LIGNES][COLONNES], int progression[6]) {
    int modif = 0;
    int items_a_detruire[LIGNES][COLONNES] = {0}; 
    int creation_special[LIGNES][COLONNES] = {0}; 

    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            int val = grille[i][j];
            if (val == 0) continue;
            
            int typeBase = val % 10; 

            // check longueurs
            int lenH = 0, k = j;
            while (k < COLONNES && (grille[i][k] % 10) == typeBase) { lenH++; k++; }
            int lenV = 0, l = i;
            while (l < LIGNES && (grille[l][j] % 10) == typeBase) { lenV++; l++; }

            // gros carre 4x4
            if (i <= LIGNES - 4 && j <= COLONNES - 4) {
                int isBigSquare = 1;
                for(int r = 0; r < 4; r++) {
                    for(int c = 0; c < 4; c++) {
                        if ((grille[i+r][j+c] % 10) != typeBase) {
                            isBigSquare = 0; break;
                        }
                    }
                    if (!isBigSquare) break;
                }

                if (isBigSquare) {
                    for(int r = 0; r < 4; r++) {
                        for(int c = 0; c < 4; c++) {
                            items_a_detruire[i+r][j+c] = 1;
                        }
                    }
                    modif = 1;
                }
            }

            // ligne 6+
            if (lenH >= 6 || lenV >= 6) {
                effet_supprimer_couleur(grille, progression, typeBase);
                gravite(grille);
                modif_grille(grille);
                resoudre_plateau(grille, progression); 
                return 1;
            }

            // croix
            if (lenH >= 5 && lenV >= 5) {
                effet_croix_explosion(grille, progression, i, j);
                gravite(grille);
                modif_grille(grille);
                resoudre_plateau(grille, progression);
                return 1;
            }

            // ligne 5 -> bombe
            if (lenH == 5) {
                for(int m=0; m<lenH; m++) items_a_detruire[i][j+m] = 1;
                creation_special[i][j+2] = typeBase + 10; 
                items_a_detruire[i][j+2] = 0; 
                modif = 1;
            }
            else if (lenV == 5) {
                for(int m=0; m<lenV; m++) items_a_detruire[i+m][j] = 1;
                creation_special[i+2][j] = typeBase + 10;
                items_a_detruire[i+2][j] = 0;
                modif = 1;
            }

            // match 3 ou 4
            else {
                // horizontal
                if (lenH >= 3) {
                    int a_une_bombe = 0;
                    for (int m = 0; m < lenH; m++) {
                        if (grille[i][j+m] > 10) a_une_bombe = 1;
                        items_a_detruire[i][j+m] = 1;
                    }
                    // si y a une bombe
                    if (a_une_bombe) {
                        effet_bombe(grille, progression, i, j + 1);
                        gravite(grille);
                        modif_grille(grille);
                        resoudre_plateau(grille, progression);
                        return 1;
                    }
                    modif = 1;
                }
                // vertical
                if (lenV >= 3) {
                    int a_une_bombe = 0;
                    for (int m = 0; m < lenV; m++) {
                        if (grille[i+m][j] > 10) a_une_bombe = 1;
                        items_a_detruire[i+m][j] = 1;
                    }
                    if (a_une_bombe) {
                        effet_bombe(grille, progression, i + 1, j);
                        gravite(grille);
                        modif_grille(grille);
                        resoudre_plateau(grille, progression);
                        return 1;
                    }
                    modif = 1;
                }
            }

            // petit carre 2x2
            if (i < LIGNES-1 && j < COLONNES-1) {
                if ((grille[i][j]%10) == typeBase && (grille[i+1][j]%10) == typeBase &&
                    (grille[i][j+1]%10) == typeBase && (grille[i+1][j+1]%10) == typeBase) {
                        items_a_detruire[i][j] = 1; items_a_detruire[i+1][j] = 1;
                        items_a_detruire[i][j+1] = 1; items_a_detruire[i+1][j+1] = 1;
                        modif = 1;
                }
            }
        }
    }

    // maj grille
    if (modif) {
        // 1. suppression
        for (int i = 0; i < LIGNES; i++) {
            for (int j = 0; j < COLONNES; j++) {
                if (items_a_detruire[i][j]) {
                    int c = grille[i][j] % 10;
                    if (c != 0) progression[c]++;
                    grille[i][j] = 0;
                }
            }
        }
        // 2. creation speciaux
        for (int i = 0; i < LIGNES; i++) {
            for (int j = 0; j < COLONNES; j++) {
                if (creation_special[i][j] != 0) grille[i][j] = creation_special[i][j];
            }
        }
        
        // 3. gravite
        gravite(grille);
        modif_grille(grille);
        
        // 4. recursif
        resoudre_plateau(grille, progression); 
    }
    return modif;
}

// objectifs

void genererObjectifs(int niveau, int objectifs[6]) {
    // reset
    for(int k=0; k<6; k++) objectifs[k] = 0;
    
    int total_cases = LIGNES * COLONNES;
    
    // difficulte
    float ratio = 0.10 + (niveau * 0.025); 
    if(ratio > 0.8) ratio = 0.8;
    
    int total_a_vider = total_cases * ratio;
    if (total_a_vider < 6) total_a_vider = 6;

    // nb objectifs
    int nb_objectifs = 2; 
    if (niveau >= 3) nb_objectifs = 3; 
    if (niveau >= 7) nb_objectifs = 4; 

    // choix couleurs
    int couleurs_utilisees[6] = {0}; 
    int count = 0;

    while (count < nb_objectifs) {
        int c = (rand() % 5) + 1;
        if (couleurs_utilisees[c] == 0) {
            couleurs_utilisees[c] = 1;
            objectifs[c] = total_a_vider / nb_objectifs;
            if (objectifs[c] < 1) objectifs[c] = 1; 
            count++;
        }
    }
}

int verif_victoire(int objectifs[6], int progression[6]) {
    for (int i = 1; i <= 5; i++) {
        if (progression[i] < objectifs[i]) return 0; // pas fini
    }
    return 1; 
}