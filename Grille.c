#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stddef.h>
#include <locale.h>
#define LIGNES 9
#define COLONNES 9

void menu() {
    printf("\n--- MENU DU JEU ---\n");
    printf("1. Lire les règles du jeu\n");
    printf("2. Commencer une nouvelle partie (à partir du 1er niveau)\n");
    printf("3. Reprendre une partie (au dernier niveau enregistré)\n");
    printf("4. Quitter\n");
    printf("-------------------\n");
    printf("Entrez votre choix (1-4) : ");
    // Note : Il faudrait lire la saisie de l'utilisateur ici, par exemple avec scanf
    int choix;
    scanf("%d", &choix);
    switch (choix) {
        case 1:
            printf("1. Lire les règles du jeu\n");
            break;
        case 2:
            printf("2. Commencer une nouvelle partie (à partir du 1er niveau)\n");
            break;
        case 3: // Si choix vaut 2 OU 3
            printf("3. Reprendre une partie (au dernier niveau enregistré)\n");
            break; // L'exécution s'arrête ici
        default:
            printf("Choix invalide.\n");
            break;
    }
}
void modif_grille(int grille[LIGNES][COLONNES]) {
    // Initialise la matrice avec des randoms
    for (int i = 0; i < LIGNES; i++)
    {
        for (int j = 0; j < COLONNES; j++)
        {
                if (grille[i][j] == 0)
            {
                grille[i][j] = (rand() % 4 ) + 1 ;
            }
        }
    }
}

void gravite(int grille[LIGNES][COLONNES]) {
    // Pour chaque colonne, on fait "tomber" les symboles.
    for (int j = 0; j < COLONNES; j++) {
        int k = LIGNES - 1; // Pointeur pour la prochaine position vide en partant du bas.

        // On parcourt la colonne du bas vers le haut.
        for (int i = LIGNES - 1; i >= 0; i--) {
            // Si on trouve un symbole non nul...
            if (grille[i][j] != 0) {
                // ...on le déplace vers la position la plus basse disponible (k).
                int temp = grille[i][j];
                grille[i][j] = 0; // On vide son ancienne case.
                grille[k][j] = temp; // On le place dans la nouvelle case.
                k--; // On décrémente k pour la prochaine case à remplir.
            }
        }
    }
}
void creation_grille(int grille[LIGNES][COLONNES]) {
    // Initialise la matrice avec des randoms
    for (int i = 0; i < LIGNES; i++)
    {
        for (int j = 0; j < COLONNES; j++)
        {
                grille[i][j] = (rand() % 4) + 1;
            }
        }
    }

void afficher_grille(int grille[LIGNES][COLONNES]){
    
    printf("tableau 2d :\n");
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            printf("%d ", grille[i][j]);
        }
        printf("\n");
    }
}


int verif_combi_horizontale(int grille[LIGNES][COLONNES]) {
    int combi_trouvee = 0; // 0 = non, 1 = oui
    // On itère sur les colonnes de 1 à COLONNES-2 pour avoir accès
    // à j-1 et j+1 sans sortir des limites du tableau.
    for (int i = 0; i < LIGNES; i++){
        for (int j = 1; j < COLONNES - 1; j++){
            
            // On vérifie une combinaison de symboles identiques ET non nuls
            if (grille[i][j] != 0 && grille[i][j] == grille[i][j-1] && grille[i][j] == grille[i][j+1] )
            {
                // Assignation correcte pour mettre les 3 éléments à 0
                grille[i][j] = grille[i][j-1] = grille[i][j+1] = 0;
                combi_trouvee = 1;
            }
        }       
    }
    return !combi_trouvee; // Retourne 1 si aucune combi (combi_trouvee=0), sinon 0
}

int verif_combi_verticale(int grille[LIGNES][COLONNES]) {
    int combi_trouvee = 0; // 0 = non, 1 = oui
    for (int i = 1; i < LIGNES-1; i++){
        for (int j = 0; j < COLONNES; j++){
            
            // On vérifie une combinaison de symboles identiques ET non nuls
            if ( grille[i][j] != 0 && grille[i][j] == grille[i+1][j] && grille[i][j] == grille[i-1][j] )
            {
                // Assignation correcte pour mettre les 3 éléments à 0
                grille[i][j] = grille[i-1][j] = grille[i+1][j] = 0;
                combi_trouvee = 1;
            }
        }       
    }
    return !combi_trouvee; // Retourne 1 si aucune combi (combi_trouvee=0), sinon 0
}

void 
// test ?

int main(void) {
    // au début du programme, sinon vous obtiendrez la même grille à chaque exécution.
    srand(time(NULL)); 

    int grille[LIGNES][COLONNES];
    creation_grille(grille);
    afficher_grille(grille);

    int combi_v, combi_h;
    do {
        combi_h = verif_combi_horizontale(grille);
        combi_v = verif_combi_verticale(grille);

        // Si au moins une combinaison a été trouvée (retour de 0)
        if (combi_h == 0 || combi_v == 0) {
            printf("\nCombinaison(s) trouvée(s) ! Application de la gravité.\n");
            gravite(grille);
            printf("Remplissage des cases vides...\n");
            modif_grille(grille);
            afficher_grille(grille);
        }

    // On continue tant qu'on trouve des combinaisons
    } while (combi_h == 0 || combi_v == 0);

    printf("\nPlus aucune combinaison possible. Fin de la boucle.\n");
    afficher_grille(grille);
    
    return (0);
}
