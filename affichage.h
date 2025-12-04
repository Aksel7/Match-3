#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <stdio.h>

/*
 * Affiche les informations du niveau :
 * - coups restants
 * - score actuel
 * - objectifs restants (tableau de 5 pierres)
 *
 * Entrees :
 *   - int coups : nombre de coups restants
 *   - int score : score actuel du joueur
 *   - int objectif[5] : tableau contenant les objectifs restant pour chaque pierre
 *
 * Sortie :
 *   - aucune (void)
 */
void afficherInfosNiveau(int coups, int score, int temps, int vies, int objectif[5]);

#endif
