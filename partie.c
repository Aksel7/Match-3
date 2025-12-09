#include <stdio.h>
#include "partie.h"

#define VIES_INIT 10  // Nombre initial de vies



// Initialiser les objectifs d'un niveau    _______________________________________

void initialiserContrat(int niveau) {
    printf("\n===== Initialisation du niveau %d =====\n", niveau);
    printf("Objectifs du niveau %d : aligner 3 bonbons ou plus\n", niveau);
    printf("=====  Bonne chance a toi !  =====\n\n");
}




// Sauvegarder la progression de la partie     ____________________________________

void sauvegarderPartie(int niveau, int vies) {
    printf("\n--- Sauvegarde ---\n");
    printf("Partie sauvegardee : Niveau %d | Vies restantes : %d\n", niveau, vies);
    printf("/////////////////////////////\n\n");
}




// Boucle d'un niveau     _________________________________________________________

void jouerNiveau(int niveau, int *vies) {
    printf("\n==============================\n");
    printf("           NIVEAU %d\n", niveau);
    printf("==============================\n");

    // Initialiser le contrat du niveau
    initialiserContrat(niveau);

    // Simulation du jeu
    printf("Simulation du jeu du niveau...\n");
    printf("Vous perdez une vie pour tester la mecanique.\n\n");

    (*vies)--;  // iciii On retire une vie pour tester

    if (*vies > 0) {
        printf("Passage au niveau suivant !\n");
        printf("Vies restantes : %d\n", *vies);
    } else {
        printf("Vous n'avez plus de vies. Partie terminee.\n");
    }

    // Proposition de sauvegarde
    int choixSauvegarde;
    printf("\nVoulez-vous sauvegarder la partie ?\n1=Oui, 0=Non\n: ");
    scanf("%d", &choixSauvegarde);

    if (choixSauvegarde == 1) {
        sauvegarderPartie(niveau, *vies);
    } else {
        printf("\nSauvegarde annulee.\n");
    }

    printf("\n------------------------------------\n");
}




// Boucle principale de la partie __________________________________________________

void jouerPartie() {
    int niveau = 1;      // Niveau de départ
    int vies = VIES_INIT;  // Vies initiales

    printf("\n>>>>>>> Demarrage d'une nouvelle partie <<<<<<<\n");

    // Boucle tant que le joueur a des vies
    while (vies > 0) {
        jouerNiveau(niveau, &vies);
        niveau++;
    }

    printf("\n=== Fin de la partie ===\n\n");
}




// Charger une partie sauvegardee _________________________________________________

void chargerSauvegarde() {
    int niveau = 2; // exemple de niveau sauvegarde
    int vies = 2;   // exemple de vies sauvegardees

    printf("\n>>> Chargement de la sauvegarde <<<\n");
    printf("Fonction chargerSauvegarde() appelee\n");
    printf("Reprise au niveau %d avec %d vies\n", niveau, vies);

    // Relance la partie à partir du niveau et vies sauvegardes
    while (vies > 0) {
        jouerNiveau(niveau, &vies);
        niveau++;
    }

    printf("\n=== Fin de la partie sauvegardee ===\n\n");
}
