#ifndef PARTIE_H
#define PARTIE_H

// Boucle principale du jeu : navigation entre les niveaux
void jouerPartie();

// Boucle d'un niveau : gestion des coups, victoire/défaite
void jouerNiveau(int niveau, int *vies);

// Charger une partie sauvegardée
void chargerSauvegarde();

// Sauvegarder la progression du joueur
void sauvegarderPartie(int niveau, int vies);

// Initialiser les objectifs d'un niveau
void initialiserContrat(int niveau);

#endif
