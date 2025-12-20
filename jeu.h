#ifndef JEU_H
#define JEU_H

#include "config.h" 

void creation_grille(int grille[LIGNES][COLONNES]);
void modif_grille(int grille[LIGNES][COLONNES]);
void gravite(int grille[LIGNES][COLONNES]);

int resoudre_plateau(int grille[LIGNES][COLONNES], int progression[6]);

void genererObjectifs(int niveau, int objectifs[6]);
int verif_victoire(int objectifs[6], int progression[6]);

#endif