#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// On inclut votre header personnalisé (qui inclut déjà windows.h et conio.h)
#include "affichage_console.h"

// --- CONSTANTES ---
#define LIGNES 9
#define COLONNES 9
#define FICHIER_SAUVEGARDE "sauvegarde.txt"
#define VIES_INIT 3
#define TEMPS_LIMITE 60 // Temps en secondes par niveau
#define prct_diff 0.1 // Pourcentage lié au contrat généré 



// Noms pour l'affichage textuel des objectifs
const char* NOM_COULEURS[] = {"", "VERT", "ROUGE", "BLEU", "JAUNE", "MAGENTA"};

// MAPPING DES COULEURS (Basé sur votre enum COLORS du header)
// Index 0 (Vide) -> BLACK
// Index 1 (Vert) -> LIGHTGREEN
// Index 2 (Rouge) -> LIGHTRED
// Index 3 (Bleu) -> LIGHTBLUE
// Index 4 (Jaune) -> YELLOW
// Index 5 (Magenta) -> LIGHTMAGENTA
const int CODE_COULEURS[] = { BLACK, LIGHTGREEN, LIGHTRED, LIGHTBLUE, YELLOW, LIGHTMAGENTA };

// ==========================================
// PARTIE 1 : LOGIQUE DU PLATEAU (Moteur)
// ==========================================

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

/* int verif_combi_horizontale(int grille[LIGNES][COLONNES], int progression[6]) {
    int combo = 0;
    for (int i = 0; i < LIGNES; i++){
        for (int j = 1; j < COLONNES - 1; j++){
            if (grille[i][j] != 0 && grille[i][j] == grille[i][j-1] && grille[i][j] == grille[i][j+1] ) {
                int couleur = grille[i][j];
                
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
} */
// --- EFFETS SPÉCIAUX (Selon le Cahier des Charges) ---

// Effet Ligne de 6 : Supprime TOUS les items de cette couleur 
void effet_supprimer_couleur(int grille[LIGNES][COLONNES], int progression[6], int couleur) {
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if (grille[i][j] == couleur) {
                grille[i][j] = 0;
                progression[couleur]++;
            }
        }
    }
}

// Effet Croix de 9 : Supprime toute la ligne ET la colonne du centre 
void effet_croix_explosion(int grille[LIGNES][COLONNES], int progression[6], int cX, int cY) {
    // Vider la ligne
    for (int j = 0; j < COLONNES; j++) {
        if (grille[cX][j] != 0) {
            int coul = grille[cX][j];
            grille[cX][j] = 0;
            progression[coul]++;
        }
    }
    // Vider la colonne
    for (int i = 0; i < LIGNES; i++) {
        if (grille[i][cY] != 0) {
            int coul = grille[i][cY];
            grille[i][cY] = 0;
            progression[coul]++;
        }
    }
}


int resoudre_plateau(int grille[LIGNES][COLONNES], int progression[6]) {
    int modif = 0;
    int items_a_detruire[LIGNES][COLONNES] = {0}; // 0=Non, 1=Oui
    
    // --- ETAPE 1 : ANALYSE DES FIGURES ---
    
    // On parcourt chaque case pour voir si elle est le début d'une figure
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            int couleur = grille[i][j];
            if (couleur == 0) continue;

            // 1. Compter longueur Horizontale
            int lenH = 0;
            int k = j;
            while (k < COLONNES && grille[i][k] == couleur) { lenH++; k++; }

            // 2. Compter longueur Verticale
            int lenV = 0;
            int l = i;
            while (l < LIGNES && grille[l][j] == couleur) { lenV++; l++; }

            // --- DETECTION DES COMBOS SPÉCIAUX ---

            // CAS A : Ligne de 6 ou plus (Horizontalement) 
            if (lenH >= 6) {
                effet_supprimer_couleur(grille, progression, couleur);
                return 1; // On relance direct la gravité car le plateau a beaucoup changé
            }
            // CAS B : Ligne de 6 ou plus (Verticalement)
            if (lenV >= 6) {
                effet_supprimer_couleur(grille, progression, couleur);
                return 1;
            }

            // CAS C : Croix de 9 items (Intersection de Ligne et Colonne) 
            // On vérifie si on est à une intersection. 
            // Note: C'est complexe à détecter parfaitement, ici on simplifie :
            // Si à cette case (i,j) on a une longue ligne horizontale ET verticale qui se croisent
            // Ex: Une ligne de 5 et une colonne de 5 qui se croisent font 9 items (5+5-1 centre)
            if (lenH >= 5 && lenV >= 5) {
                // On considère que le centre de la croix est ici ou proche
                effet_croix_explosion(grille, progression, i, j);
                return 1;
            }

            // --- DETECTION STANDARD (Match 3, 4, 5) ---
            
            // Marquage Horizontal
            if (lenH >= 3) {
                for (int k = 0; k < lenH; k++) items_a_detruire[i][j + k] = 1;
                modif = 1;
            }
            
            // Marquage Vertical
            if (lenV >= 3) {
                for (int k = 0; k < lenV; k++) items_a_detruire[i + k][j] = 1;
                modif = 1;
            }
            
            // CAS D : Carré (Bonus PDF: Carré de 4 items) 
            // Note: Le PDF dit "4*4", mais c'est surement une erreur pour "Carré de 4" (2x2)
            // car 4x4 est impossible. On code le 2x2.
            if (i < LIGNES-1 && j < COLONNES-1) {
                if (grille[i][j] == couleur && grille[i+1][j] == couleur &&
                    grille[i][j+1] == couleur && grille[i+1][j+1] == couleur) {
                        items_a_detruire[i][j] = 1;
                        items_a_detruire[i+1][j] = 1;
                        items_a_detruire[i][j+1] = 1;
                        items_a_detruire[i+1][j+1] = 1;
                        modif = 1;
                }
            }
        }
    }

    // --- ETAPE 2 : SUPPRESSION ---
    if (modif) {
        for (int i = 0; i < LIGNES; i++) {
            for (int j = 0; j < COLONNES; j++) {
                if (items_a_detruire[i][j]) {
                    int c = grille[i][j];
                    if (c != 0) progression[c]++; // On compte pour l'objectif
                    grille[i][j] = 0; // On détruit
                }
            }
        }
        
        // --- ETAPE 3 : GRAVITÉ ET REMPLISSAGE ---
        gravite(grille);
        modif_grille(grille);
        
        // Récursivité : On revérifie si ça a créé de nouveaux combos !
        resoudre_plateau(grille, progression); 
    }

    return modif;
}

// ==========================================
// PARTIE 2 : OBJECTIFS & LOGIQUE DE NIVEAU
// ==========================================

void genererObjectifs(int niveau, int objectifs[6]) {
    // 1. Réinitialiser le tableau
    for(int k=0; k<6; k++) objectifs[k] = 0;

    // 2. Calcul du volume total de la grille
    int total_cases = LIGNES * COLONNES;

    // --- LOGIQUE D'ÉQUILIBRAGE ---
    
    // RAPPEL STATISTIQUE :
    // Sur une grille aléatoire à 5 couleurs, chaque couleur occupe naturellement ~20% de l'espace.
    // Pour qu'un objectif soit un "défi", il faut demander AU MOINS 30% ou 40% du volume total.

    if (niveau == 1) {
        // NIVEAU 1 : INTRO (Facile mais pas trivial)
        // On demande 2 couleurs pour éviter d'être bloqué si une couleur manque.
        // Objectif : Vider l'équivalent de 30% de la grille au total.
        int total_a_vider = total_cases * 0.30;
        
        objectifs[2] = total_a_vider / 2; // Rubis
        objectifs[4] = total_a_vider / 2; // Jaune (Topaze)
    }
    else if (niveau == 2) {
        // NIVEAU 2 : MONTÉE EN PUISSANCE
        // Objectif : Vider 45% de la grille.
        int total_a_vider = total_cases * 0.45;
        
        objectifs[1] = total_a_vider / 2; // Emeraude
        objectifs[3] = total_a_vider / 2; // Saphir
    }
    else if (niveau < 5) {
        // NIVEAU 3 et 4 : CHALLENGE STANDARD
        // Formule : 50% de la grille + 10% par niveau supplémentaire
        float ratio = 0.50 + (0.10 * (niveau - 3));
        int total_a_vider = total_cases * ratio;

        // Choix aléatoire de 2 couleurs différentes
        int c1 = (rand() % 5) + 1;
        int c2 = (rand() % 5) + 1;
        while(c2 == c1) c2 = (rand() % 5) + 1;

        objectifs[c1] = total_a_vider / 2;
        objectifs[c2] = total_a_vider / 2;
    }
    else {
        // NIVEAU 5+ : DIFFICILE (3 COULEURS)
        // À ce stade, on demande de vider presque l'équivalent de toute la grille (100%+)
        // Le joueur doit jouer vite et faire des combos.
        float ratio = 0.80 + (0.15 * (niveau - 5)); 
        int total_a_vider = total_cases * ratio;

        // Choix aléatoire de 3 couleurs différentes
        int c1 = (rand() % 5) + 1;
        int c2 = (rand() % 5) + 1;
        while(c2 == c1) c2 = (rand() % 5) + 1;
        int c3 = (rand() % 5) + 1;
        while(c3 == c1 || c3 == c2) c3 = (rand() % 5) + 1;

        objectifs[c1] = total_a_vider / 3;
        objectifs[c2] = total_a_vider / 3;
        objectifs[c3] = total_a_vider / 3;
    }
}

int verif_victoire(int objectifs[6], int progression[6]) {
    for (int i = 1; i <= 5; i++) {
        if (progression[i] < objectifs[i]) {
            return 0; 
        }
    }
    return 1; 
}

// ==========================================
// PARTIE 3 : AFFICHAGE & INTERFACE (Refaite avec affichage_console)
// ==========================================

void afficherInfosHUD(char pseudo[], int niveau, int coups, int vies, int objectifs[6], int progression[6], int tempsRestant) {
    text_color(WHITE);
    printf("=========================================================\n");
    
    printf(" JOUEUR: %s | NIV: %d | VIES: %d | COUPS: %d | TEMPS: ", pseudo, niveau, vies, coups);
    
    // Changement de couleur dynamique pour le temps
    if (tempsRestant <= 10) text_color(LIGHTRED);
    else text_color(LIGHTGREEN);
    
    printf("%ds", tempsRestant);
    text_color(WHITE); // Reset
    
    printf("\n=========================================================\n");
    printf(" OBJECTIFS :\n");
    
    int a_un_objectif = 0;
    for (int i = 1; i <= 5; i++) {
        if (objectifs[i] > 0) {
            // Affichage coloré du nom de l'objectif
            text_color(CODE_COULEURS[i]);
            printf("  %s", NOM_COULEURS[i]);
            
            text_color(WHITE);
            printf(" : %d / %d", progression[i], objectifs[i]);
            
            if (progression[i] >= objectifs[i]) {
                text_color(LIGHTGREEN);
                printf(" (OK!)");
            }
            printf("\n");
            a_un_objectif = 1;
        }
    }
    text_color(WHITE);
    if(!a_un_objectif) printf("  Aucun objectif ? (Bug)\n");

    printf("---------------------------------------------------------\n");
    printf(" [Z/Q/S/D] Deplacement | [ESPACE] Selection | [P] Abandonner\n");
}

void afficherPlateau(int grille[LIGNES][COLONNES], int cX, int cY, int selActive, int selX, int selY) {
    printf("\n");
    for (int i = 0; i < LIGNES; i++) {
        printf("    "); 
        for (int j = 0; j < COLONNES; j++) {
            
            int couleur = CODE_COULEURS[grille[i][j]];
            int estCurseur = (i == cX && j == cY);
            int estSelectionne = (selActive && i == selX && j == selY);

            // Gestion de l'affichage curseur/sélection
            if (estCurseur) {
                text_color(WHITE);
                if (selActive && estSelectionne) printf("<");
                else printf("[");
                
                text_color(couleur);
                printf("%d", grille[i][j]);
                
                text_color(WHITE);
                if (selActive && estSelectionne) printf("> ");
                else printf("] ");

            } else if (estSelectionne) {
                text_color(WHITE);
                printf(" *");
                text_color(couleur);
                printf("%d", grille[i][j]);
                text_color(WHITE);
                printf("* ");
            } else {
                text_color(DARKGRAY); // Crochets gris pour le fond
                printf(" ");
                text_color(couleur); // Chiffre en couleur
                printf("%d", grille[i][j]);
                text_color(DARKGRAY);
                printf("  ");
            }
        }
        printf("\n\n");
    }
    text_color(WHITE); // Toujours remettre en blanc à la fin
}

// ==========================================
// PARTIE 4 : BOUCLE DE NIVEAU
// ==========================================

int lancerNiveau(char pseudo[], int niveau, int *vies) {
    int grille[LIGNES][COLONNES];
    
    // --- ADAPTATION DYNAMIQUE ---
    // Sur une grille 9x9 (81 cases) -> ~20 coups
    // Sur une grille 25x45 (1125 cases) -> ~35 coups (les combos sont plus gros, donc pas besoin de 100 coups)
    
    int total_cases = LIGNES * COLONNES;
    
    // Formule simple : Base de 15 coups + 1 coup par tranche de 50 cases, + bonus par niveau
    int coups = 15 + (total_cases / 50) + (niveau * 2);
    
    // Plafond pour éviter d'avoir trop de coups sur la grille géante
    if (coups > 40) coups = 40; // Le PDF suggère environ 30 coups 

    int objectifs[6] = {0};
    int progression[6] = {0};
    
    // ... suite du code ...
    
    creation_grille(grille);
    int poubelle[6] = {0}; 
    resoudre_plateau(grille, poubelle); 

    genererObjectifs(niveau, objectifs);

    int cX = 0, cY = 0;
    int selActive = 0, sX = -1, sY = -1;
    int running = 1;

    // Timer
    time_t debut = time(NULL);
    int tempsRestant = TEMPS_LIMITE;
    int dernierTempsAffiche = TEMPS_LIMITE;
    int besoinRafraichissement = 1;

    // On cache le curseur pour un rendu plus "Jeu vidéo"
    hide_cursor();

    while (running && coups > 0 && tempsRestant > 0) {
        
        int ecoule = (int)difftime(time(NULL), debut);
        tempsRestant = TEMPS_LIMITE - ecoule;

        if (tempsRestant != dernierTempsAffiche) {
            dernierTempsAffiche = tempsRestant;
            besoinRafraichissement = 1;
        }

        // 1. Vérif Victoire
        if (verif_victoire(objectifs, progression)) {
            clrscr(); // Utilisation de la nouvelle fonction
            if(tempsRestant < 0) tempsRestant = 0;
            afficherInfosHUD(pseudo, niveau, coups, *vies, objectifs, progression, tempsRestant);
            afficherPlateau(grille, cX, cY, selActive, sX, sY);
            
            text_color(LIGHTGREEN);
            printf("\n\n*** VICTOIRE ! Tous les objectifs atteints ! ***\n");
            text_color(WHITE);
            show_cursor();
            system("pause");
            return 1;
        }

        // 2. Affichage
        if (besoinRafraichissement) {
            clrscr(); // Utilisation de la nouvelle fonction
            if(tempsRestant < 0) tempsRestant = 0;
            afficherInfosHUD(pseudo, niveau, coups, *vies, objectifs, progression, tempsRestant);
            afficherPlateau(grille, cX, cY, selActive, sX, sY);
            besoinRafraichissement = 0;
        }

        // 3. Gestion des Entrées
        // kbhit() et getch() sont déclarés dans votre header (via conio.h ou réimplémentation)
        if (kbhit()) { 
            char touche = getch(); 
            besoinRafraichissement = 1; 

            switch (touche) {
                case 'p': 
                    show_cursor();
                    return 0; // Abandon
                case 'z': if (cX > 0) cX--; break;
                case 's': if (cX < LIGNES - 1) cX++; break;
                case 'q': if (cY > 0) cY--; break;
                case 'd': if (cY < COLONNES - 1) cY++; break;
                case ' ': 
                    if (!selActive) {
                        selActive = 1; sX = cX; sY = cY;
                    } else {
                        int dist = abs(cX - sX) + abs(cY - sY);
                        if (dist == 1) {
                            int tmp = grille[cX][cY]; grille[cX][cY] = grille[sX][sY]; grille[sX][sY] = tmp;
                            
                            int combo = resoudre_plateau(grille, progression);
                            
                            if (combo) {
                                coups--;
                                selActive = 0;
                            } else {
                                // Mouvement invalide (pas de combo), on annule
                                tmp = grille[cX][cY]; grille[cX][cY] = grille[sX][sY]; grille[sX][sY] = tmp;
                                selActive = 0;
                            }
                        } else {
                            // Clic trop loin ou sur soi-même, on désélectionne
                            selActive = 0;
                        }
                    }
                    break;
            }
        } else {
            Sleep(50); // Pause 50ms pour ne pas surcharger le processeur
        }
    }

    show_cursor(); // IMPORTANT : Toujours réafficher le curseur en sortant
    printf("\n\n");
    
    text_color(LIGHTRED);
    if (tempsRestant <= 0) {
        printf("DEFAITE... Temps ecoule !\n");
    } else {
        printf("DEFAITE... Plus de coups disponibles.\n");
    }
    text_color(WHITE);
    
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

void afficherRegles() {
    clrscr();
    text_color(LIGHTCYAN);
    printf("=========================================================\n");
    printf("                  REGLES DU JEU : ECE HEROES             \n");
    printf("=========================================================\n\n");
    text_color(WHITE);
    printf("1. PRINCIPE :\n");
    printf("   Associez au moins 3 items identiques pour les faire\n");
    printf("   disparaitre et remplir votre contrat.\n\n");
    printf("2. COMMANDES :\n");
    printf("   - Z/Q/S/D : Deplacer le curseur.\n");
    printf("   - ESPACE  : Selectionner/Permuter un item.\n");
    printf("   - P       : Abandonner le niveau en cours.\n\n");
    printf("3. OBJECTIFS :\n");
    printf("   Chaque niveau a un contrat (nombre d'items a eliminer)\n");
    printf("   a remplir en un temps et un nombre de coups limites.\n\n");
    printf("4. VIES :\n");
    printf("   Vous commencez avec %d vies. Echouer a un niveau vous\n", VIES_INIT);
    printf("   fait perdre une vie.\n\n");
    printf("=========================================================\n");
    printf("Appuyez sur une touche pour revenir au menu...");
    getch();
}

int main() {
    srand(time(NULL));
    char pseudo[50];
    int niveau = 1;
    int vies = VIES_INIT;
    int choix;
    int continuerMenu = 1;

    // Titre de la fenêtre
    SetConsoleTitle("ECE HEROES - Match 3");

    while (continuerMenu) {
        clrscr();
        text_color(YELLOW);
        printf("--- ECE HEROES : MENU PRINCIPAL ---\n");
        text_color(WHITE);
        printf("1. Lire les regles du jeu\n");
        printf("2. Commencer une nouvelle partie\n");
        printf("3. Reprendre une partie (Charger)\n");
        printf("4. Quitter\n");
        printf("\nChoix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                afficherRegles();
                break;
            case 2:
                printf("Entrez votre pseudo : ");
                scanf("%s", pseudo);
                vies = VIES_INIT;
                niveau = 1;
                continuerMenu = 0; 
                break;
            case 3:
                printf("Entrez votre pseudo : ");
                scanf("%s", pseudo);
                if (chargerSauvegarde(pseudo, &niveau, &vies)) {
                    printf("Sauvegarde trouvee ! Niveau %d, %d vies.\n", niveau, vies);
                    system("pause");
                    continuerMenu = 0;
                } else {
                    printf("Aucune sauvegarde pour ce pseudo. Reessayez.\n");
                    system("pause");
                }
                break;
            case 4:
                printf("Merci d'avoir joue !\n");
                return 0;
            default:
                printf("Choix invalide.\n");
                system("pause");
        }
    }

    // --- Boucle de Jeu ---
    while (vies > 0) {
        if (lancerNiveau(pseudo, niveau, &vies)) {
            clrscr();
            printf("Sauvegarder ? (1=Oui/0=Non) : ");
            int sv;
            scanf("%d", &sv);
            if (sv == 1) sauvegarderPartie(pseudo, niveau + 1, vies);
            niveau++;
        } else {
            vies--;
            if (vies > 0) {
                clrscr();
                text_color(LIGHTRED);
                printf("Il vous reste %d vies.\n", vies);
                text_color(WHITE);
                printf("Recommencer ? (1=Oui/0=Non) : ");
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