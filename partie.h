#ifndef PARTIE_H
#define PARTIE_H

#include "config.h"

// fonctions de session
void sauvegarderPartie(char pseudo[], int niveau, int vies);
int chargerSauvegarde(char pseudo[], int *niveau, int *vies);
int lancerNiveau(char pseudo[], int niveau, int *vies, int musiqueActive);

#endif