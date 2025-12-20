#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "config.h"
#include "affichage_console.h" 

// --- Prototypes des  utilitaires ---
void text_color(int c);
void bg_color(int c);
void hide_cursor();
void show_cursor();
void clrscr();

// --- Prototypes des fonctions principales ---
void afficherInfosHUD(char pseudo[], int niveau, int coups, int vies, int objectifs[6], int progression[6], int tempsRestant);
void afficherPlateau(int grille[LIGNES][COLONNES], int cX, int cY, int selActive, int selX, int selY);
void afficherRegles();

#endif