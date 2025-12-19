#ifndef PARTIE_H
#define PARTIE_H

void jouerPartie();
void jouerNiveau(int niveau, int *vies);

void sauvegarderPartie(char pseudo[], int niveau, int vies);
int chargerSauvegarde(char pseudo[], int *niveau, int *vies);

void initialiserContrat(int niveau);

#endif
