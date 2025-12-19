#include <stdio.h>
#include <string.h>
#include "partie.h"

#define VIES_INIT 10
#define FICHIER_SAUVEGARDE "sauvegarde.txt"

// ------------------ CONTRAT ------------------

void initialiserContrat(int niveau) {
    printf("\n===== Niveau %d =====\n", niveau);
    printf("Objectif : aligner des items\n");
    printf("=====================\n");
}

// ------------------ SAUVEGARDE ------------------

void sauvegarderPartie(char pseudo[], int niveau, int vies) {
    FILE *f = fopen(FICHIER_SAUVEGARDE, "a");

    if (f == NULL) {
        printf("Erreur ouverture fichier sauvegarde\n");
        return;
    }

    fprintf(f, "%s %d %d\n", pseudo, niveau, vies);
    fclose(f);

    printf("Sauvegarde reussie !\n");
}

// ------------------ CHARGEMENT ------------------

int chargerSauvegarde(char pseudo[], int *niveau, int *vies) {
    FILE *f = fopen(FICHIER_SAUVEGARDE, "r");
    char pseudoLu[50];
    int niveauLu, viesLues;

    if (f == NULL) {
        printf("Aucune sauvegarde trouvee.\n");
        return 0;
    }

    while (fscanf(f, "%s %d %d", pseudoLu, &niveauLu, &viesLues) == 3) {
        if (strcmp(pseudoLu, pseudo) == 0) {
            *niveau = niveauLu;
            *vies = viesLues;
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

// ------------------ JEU ------------------

void jouerNiveau(int niveau, int *vies) {
    printf("\n--- NIVEAU %d ---\n", niveau);
    initialiserContrat(niveau);

    printf("Simulation du niveau...\n");
    printf("Perte d'une vie pour test.\n");

    (*vies)--;

    if (*vies > 0) {
        printf("Niveau termine. Vies restantes : %d\n", *vies);
    } else {
        printf("Plus de vies !\n");
    }
}

// ------------------ PARTIE ------------------

void jouerPartie() {
    char pseudo[50];
    int choix;
    int niveau = 1;
    int vies = VIES_INIT;

    printf("Entrez votre pseudo : ");
    scanf("%s", pseudo);

    printf("\n1. Nouvelle partie\n2. Reprendre partie\nChoix : ");
    scanf("%d", &choix);

    if (choix == 2) {
        if (!chargerSauvegarde(pseudo, &niveau, &vies)) {
            printf("Sauvegarde introuvable, nouvelle partie lancee.\n");
            niveau = 1;
            vies = VIES_INIT;
        }
    }

    while (vies > 0) {
        jouerNiveau(niveau, &vies);

        if (vies <= 0) break;

        printf("\nSauvegarder ? (1=oui / 0=non) : ");
        scanf("%d", &choix);

        if (choix == 1) {
            sauvegarderPartie(pseudo, niveau, vies);
        }

        niveau++;
    }

    printf("\nFin de la partie.\n");
}
