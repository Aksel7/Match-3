#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stddef.h>
#include <locale.h>
#define LIGNES 45
#define COLONNES 25

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
void creation_grille(int grille[LIGNES][COLONNES]) {
    // Initialise la matrice avec des randoms
    for (int i = 0; i < LIGNES; i++)
    {
        for (int j = 0; j < COLONNES; j++)
        {
                grille[i][j] = 0;
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


void verif_combi_horizontale(int grille[LIGNES][COLONNES]) {
    // On itère sur les colonnes de 1 à COLONNES-2 pour avoir accès
    // à j-1 et j+1 sans sortir des limites du tableau.
    for (int i = 0; i < LIGNES; i++){
        for (int j = 1; j < COLONNES - 1; j++){
            
            // Correction de la condition d'égalité : utilise &&
            if ( grille[i][j] == grille[i][j-1] && grille[i][j] == grille[i][j+1] )
            {
                // Assignation correcte pour mettre les 3 éléments à 0
                grille[i][j] = grille[i][j-1] = grille[i][j+1] = 0;
            }
        }       
    }
}

void verif_combi_verticale(int grille[LIGNES][COLONNES]) {
    for (int i = 1; i < LIGNES-1; i++){
        for (int j = 0; j < COLONNES; j++){
            
            // Correction de la condition d'égalité : utilise &&
            if ( grille[i][j] == grille[i+1][j] && grille[i][j] == grille[i-1][j] )
            {
                // Assignation correcte pour mettre les 3 éléments à 0
                grille[i][j] = grille[i-1][j] = grille[i+1][j] = 0;
            }
        }       
    }
    
}




int main(void) {
  
 /* // TEST BONBONS / EMOJIS DANS L4AFFICHAGE CONSOLE 
  setlocale(LC_CTYPE, "fr_FR.UTF-8"); // vérifier le retour en pratique
  const wchar_t *bonbon = L"🍬";
  printf("%ls\n", mywstr); */
    // Assurez-vous d'initialiser le générateur de nombres aléatoires une seule fois
    // au début du programme, sinon vous obtiendrez la même grille à chaque exécution.
    srand(time(NULL)); 

    int grille[LIGNES][COLONNES];

    creation_grille(grille);
    //printf("Grille initiale : \n");
    // afficher_grille(grille);

    verif_combi_horizontale(grille); // Appel avec le nom corrigé
    // printf("\nGrille après vérification des combinaisons horizontales : \n");
    // afficher_grille(grille);

    verif_combi_verticale(grille); // Appel avec le nom corrigé
    // printf("\nGrille après vérification des combinaisons verticales : \n");
    // afficher_grille(grille);
    modif_grille(grille);
    afficher_grille(grille);
    return (0);
}
