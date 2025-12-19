#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h> // Windows

// --- CONSTANTES ---
#define LIGNES 9
#define COLONNES 9
#define FICHIER_SAUVEGARDE "sauvegarde_v2.txt"
#define VIES_INIT 3

// Couleurs ANSI
#define RESET   "\x1b[0m"
#define ROUGE   "\x1b[31m"
#define VERT    "\x1b[32m"
#define BLEU    "\x1b[34m"
#define JAUNE   "\x1b[33m"
#define MAGENTA "\x1b[35m"
#define BLANC   "\x1b[37m"

// Tableaux de correspondance pour l'affichage (Index 0 vide, 1=Vert, etc.)
const char* NOM_COULEURS[] = {"", "VERT", "ROUGE", "BLEU", "JAUNE", "MAGENTA"};
const char* CODE_COULEURS[] = {"", VERT, ROUGE, BLEU, JAUNE, MAGENTA};

// ==========================================
// PARTIE 1 : LOGIQUE DU PLATEAU (Moteur)
// ==========================================

void effacerEcran(void) {
    system("cls");
}

void creation_grille(int grille[LIGNES][COLONNES]) {
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            grille[i][j] = (rand() % 5) + 1;
        }
    }
}

void modif_grille(int grille[LIGNES][COLONNES]) {
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if (grille[i][j] == 0) {
                grille[i][j] = (rand() % 5) + 1;
            }
        }
    }
}

void gravite(int grille[LIGNES][COLONNES]) {
    for (int j = 0; j < COLONNES; j++) {
        int k = LIGNES - 1; 
        for (int i = LIGNES - 1; i >= 0; i--) {
            if (grille[i][j] != 0) {
                int temp = grille[i][j];
                grille[i][j] = 0;
                grille[k][j] = temp;
                k--;
            }
        }
    }
}

// MODIFICATION : On passe le tableau 'progression' pour compter les couleurs détruites
int verif_combi_horizontale(int grille[LIGNES][COLONNES], int progression[6]) {
    int combo = 0;
    for (int i = 0; i < LIGNES; i++){
        for (int j = 1; j < COLONNES - 1; j++){
            // Si une ligne de 3 est détectée
            if (grille[i][j] != 0 && grille[i][j] == grille[i][j-1] && grille[i][j] == grille[i][j+1] ) {
                int couleur = grille[i][j];
                
                // On compte et on supprime (si pas déjà supprimé par une croix)
                if(grille[i][j-1] != 0) { progression[couleur]++; grille[i][j-1] = 0; }
                if(grille[i][j] != 0)   { progression[couleur]++; grille[i][j] = 0; }
                if(grille[i][j+1] != 0) { progression[couleur]++; grille[i][j+1] = 0; }
                
                combo = 1;
            }
        }       
    }
    return combo;
}

int verif_combi_verticale(int grille[LIGNES][COLONNES], int progression[6]) {
    int combo = 0;
    for (int i = 1; i < LIGNES-1; i++){
        for (int j = 0; j < COLONNES; j++){
            // Si une colonne de 3 est détectée
            if ( grille[i][j] != 0 && grille[i][j] == grille[i+1][j] && grille[i][j] == grille[i-1][j] ) {
                int couleur = grille[i][j];

                if(grille[i-1][j] != 0) { progression[couleur]++; grille[i-1][j] = 0; }
                if(grille[i][j] != 0)   { progression[couleur]++; grille[i][j] = 0; }
                if(grille[i+1][j] != 0) { progression[couleur]++; grille[i+1][j] = 0; }

                combo = 1;
            }
        }       
    }
    return combo;
}

// Retourne 1 si quelque chose a changé (combo), 0 sinon
int resoudre_plateau(int grille[LIGNES][COLONNES], int progression[6]) {
    int changed = 0;
    int c_h, c_v;
    do {
        c_h = verif_combi_horizontale(grille, progression);
        c_v = verif_combi_verticale(grille, progression);

        if (c_h || c_v) {
            gravite(grille);
            modif_grille(grille);
            changed = 1;
        }
    } while (c_h || c_v);
    
    return changed;
}

// ==========================================
// PARTIE 2 : OBJECTIFS & LOGIQUE DE NIVEAU
// ==========================================

// Génère des objectifs aléatoires basés sur la difficulté du niveau
void genererObjectifs(int niveau, int objectifs[6]) {
    // Reset
    for(int k=0; k<6; k++) objectifs[k] = 0;

    // Niveau 1 : Une seule couleur
    if (niveau == 1) {
        objectifs[2] = 10; // 10 Rouges
    }
    // Niveau 2 : Deux couleurs
    else if (niveau == 2) {
        objectifs[1] = 8; // 8 Verts
        objectifs[3] = 8; // 8 Bleus
    }
    // Niveaux supérieurs : Aléatoire mais croissant
    else {
        int quantiteTotale = 15 + (niveau * 3); // Plus le niveau monte, plus il faut détruire
        // On répartit sur 2 ou 3 couleurs au hasard
        int c1 = (rand() % 5) + 1;
        int c2 = (rand() % 5) + 1;
        while(c2 == c1) c2 = (rand() % 5) + 1; // Assurer 2 couleurs différentes

        objectifs[c1] = quantiteTotale / 2;
        objectifs[c2] = quantiteTotale / 2;
    }
}

// Vérifie si TOUS les objectifs sont atteints
int verif_victoire(int objectifs[6], int progression[6]) {
    for (int i = 1; i <= 5; i++) {
        if (progression[i] < objectifs[i]) {
            return 0; // Pas encore fini
        }
    }
    return 1; // Victoire !
}

// ==========================================
// PARTIE 3 : AFFICHAGE & INTERFACE
// ==========================================

void afficherInfosHUD(char pseudo[], int niveau, int coups, int vies, int objectifs[6], int progression[6]) {
    printf("===================================================\n");
    printf(" JOUEUR: %s | NIVEAU: %d | VIES: %d | COUPS: %d\n", pseudo, niveau, vies, coups);
    printf("===================================================\n");
    printf(" OBJECTIFS :\n");
    
    // Affichage dynamique des objectifs
    int a_un_objectif = 0;
    for (int i = 1; i <= 5; i++) {
        if (objectifs[i] > 0) {
            // Affiche [COULEUR : Actuel / Total]
            // Si l'objectif est fini, on met un petit "OK"
            char status[10] = "";
            if (progression[i] >= objectifs[i]) strcpy(status, " (OK!)");

            printf("  %s%s : %d / %d%s%s\n", 
                CODE_COULEURS[i], NOM_COULEURS[i], 
                progression[i], objectifs[i], status, RESET);
            a_un_objectif = 1;
        }
    }
    if(!a_un_objectif) printf("  Aucun objectif ? (Bug)\n");

    printf("---------------------------------------------------\n");
    printf(" [Z/Q/S/D] Deplacement | [ESPACE] Selection | [P] Abandonner\n");
}

void afficherPlateau(int grille[LIGNES][COLONNES], int cX, int cY, int selActive, int selX, int selY) {
    printf("\n");
    for (int i = 0; i < LIGNES; i++) {
        printf("    "); 
        for (int j = 0; j < COLONNES; j++) {
            // Sélection de la couleur ANSI
            char* couleur = (char*)CODE_COULEURS[grille[i][j]];

            if (i == cX && j == cY) {
                if (selActive && i == selX && j == selY) printf("<%s%d%s> ", couleur, grille[i][j], RESET);
                else printf("[%s%d%s] ", couleur, grille[i][j], RESET);
            } else if (selActive && i == selX && j == selY) {
                printf(" %s%d%s* ", couleur, grille[i][j], RESET);
            } else {
                printf(" %s%d%s  ", couleur, grille[i][j], RESET);
            }
        }
        printf("\n\n");
    }
}

// ==========================================
// PARTIE 4 : BOUCLE DE NIVEAU
// ==========================================

int lancerNiveau(char pseudo[], int niveau, int *vies) {
    int grille[LIGNES][COLONNES];
    int coups = 20; // 20 coups pour réussir
    
    // Tableaux pour objectifs et progression (Index 1 à 5 utilisés)
    int objectifs[6] = {0};
    int progression[6] = {0};
    
    // Génération du niveau
    creation_grille(grille);
    
    // On simule une résolution pour nettoyer, mais SANS compter dans la progression
    int poubelle[6] = {0}; 
    resoudre_plateau(grille, poubelle); 

    // Définir les objectifs de ce niveau
    genererObjectifs(niveau, objectifs);

    // Variables UI
    int cX = 0, cY = 0;
    int selActive = 0, sX = -1, sY = -1;
    int running = 1;

    while (running && coups > 0) {
        // 1. Vérif Victoire
        if (verif_victoire(objectifs, progression)) {
            effacerEcran();
            afficherInfosHUD(pseudo, niveau, coups, *vies, objectifs, progression);
            afficherPlateau(grille, cX, cY, selActive, sX, sY);
            printf("\n\n*** VICTOIRE ! Tous les objectifs atteints ! ***\n");
            system("pause");
            return 1; // Niveau gagné
        }

        // 2. Affichage
        effacerEcran();
        afficherInfosHUD(pseudo, niveau, coups, *vies, objectifs, progression);
        afficherPlateau(grille, cX, cY, selActive, sX, sY);

        // 3. Input
        char touche = _getch();

        switch (touche) {
            case 'p': return 0; // Abandon
            case 'z': if (cX > 0) cX--; break;
            case 's': if (cX < LIGNES - 1) cX++; break;
            case 'q': if (cY > 0) cY--; break;
            case 'd': if (cY < COLONNES - 1) cY++; break;
            case ' ': // Action
                if (!selActive) {
                    selActive = 1; sX = cX; sY = cY;
                } else {
                    int dist = abs(cX - sX) + abs(cY - sY);
                    if (dist == 1) {
                        // Echange
                        int tmp = grille[cX][cY]; grille[cX][cY] = grille[sX][sY]; grille[sX][sY] = tmp;
                        
                        // Résolution avec mise à jour de la progression
                        int combo = resoudre_plateau(grille, progression);
                        
                        if (combo) {
                            coups--;
                            selActive = 0;
                        } else {
                            // Annulation (Mouvement invalide)
                            tmp = grille[cX][cY]; grille[cX][cY] = grille[sX][sY]; grille[sX][sY] = tmp;
                            selActive = 0;
                        }
                    } else {
                        selActive = 0;
                    }
                }
                break;
        }
    }

    printf("\n\nDEFAITE... Plus de coups disponibles.\n");
    system("pause");
    return 0; // Perdu
}

// ==========================================
// PARTIE 5 : MENU & SAUVEGARDE
// ==========================================

void sauvegarderPartie(char pseudo[], int niveau, int vies) {
    FILE *f = fopen(FICHIER_SAUVEGARDE, "a");
    if (f) {
        fprintf(f, "%s %d %d\n", pseudo, niveau, vies);
        fclose(f);
        printf("\nSauvegarde OK.\n");
    }
}

int chargerSauvegarde(char pseudo[], int *niveau, int *vies) {
    FILE *f = fopen(FICHIER_SAUVEGARDE, "r");
    char pLu[50];
    int nLu, vLu;
    int trouve = 0;
    if (!f) return 0;

    while (fscanf(f, "%s %d %d", pLu, &nLu, &vLu) == 3) {
        if (strcmp(pLu, pseudo) == 0) {
            *niveau = nLu;
            *vies = vLu;
            trouve = 1;
        }
    }
    fclose(f);
    return trouve;
}

int main() {
    srand(time(NULL));
    char pseudo[50];
    int niveau = 1;
    int vies = VIES_INIT;
    int choix;

    effacerEcran();
    printf("--- ECE HEROES : COLOR EDITION ---\n");
    printf("Entrez votre pseudo : ");
    scanf("%s", pseudo);

    printf("\n1. Nouvelle partie\n2. Charger\nChoix : ");
    scanf("%d", &choix);

    if (choix == 2) {
        if (chargerSauvegarde(pseudo, &niveau, &vies)) {
            printf("Retour au niveau %d\n", niveau);
            system("pause");
        } else {
            printf("Pas de sauvegarde. Nouveau depart.\n");
            system("pause");
        }
    }

    while (vies > 0) {
        if (lancerNiveau(pseudo, niveau, &vies)) {
            // Gagné
            printf("Sauvegarder ? (1=Oui/0=Non) : ");
            int sv;
            scanf("%d", &sv);
            if (sv == 1) sauvegarderPartie(pseudo, niveau + 1, vies);
            
            niveau++;
        } else {
            // Perdu
            vies--;
            if (vies > 0) {
                printf("Il vous reste %d vies. Recommencer ? (1=Oui/0=Non) : ", vies);
                int retry;
                scanf("%d", &retry);
                if (!retry) break;
            }
        }
    }

    printf("\nGAME OVER\n");
    system("pause");
    return 0;
}