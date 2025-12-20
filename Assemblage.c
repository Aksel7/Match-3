#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "affichage_console.h"

// --- CONSTANTES ---
#define LIGNES 20      
#define COLONNES 20    
#define FICHIER_SAUVEGARDE "sauvegarde.txt"
#define VIES_INIT 3

// --- NOMS ET COULEURS DES PIERRES PRECIEUSES ---
// Index 0 (Vide), 1 (Vert), 2 (Rouge), 3 (Bleu), 4 (Jaune), 5 (Cyan/Diamant)
const char* NOM_PIERRES[] = {"", "EMERAUDE", "RUBIS", "SAPHIR", "TOPAZE", "DIAMANT"};

// MAPPING DES COULEURS (Doit correspondre à l'ordre ci-dessus)
const int CODE_COULEURS[] = { BLACK, LIGHTGREEN, LIGHTRED, LIGHTBLUE, YELLOW, LIGHTCYAN };

// ==========================================
// PARTIE 1 : LOGIQUE DU PLATEAU
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

// --- EFFETS SPÉCIAUX ---

// 1. EFFET BOMBE DE ZONE (Rayon 1 = 3x3 cases)
void effet_bombe(int grille[LIGNES][COLONNES], int progression[6], int cX, int cY) {
    int rayon = 1; 
    for (int i = cX - rayon; i <= cX + rayon; i++) {
        for (int j = cY - rayon; j <= cY + rayon; j++) {
            if (i >= 0 && i < LIGNES && j >= 0 && j < COLONNES) {
                if (grille[i][j] != 0) {
                    int type = grille[i][j] % 10;
                    progression[type]++; 
                    grille[i][j] = 0;    
                }
            }
        }
    }
}

// 2. EFFET CROIX
void effet_croix_explosion(int grille[LIGNES][COLONNES], int progression[6], int cX, int cY) {
    for (int j = 0; j < COLONNES; j++) { // Ligne
        if (grille[cX][j] != 0) {
            int coul = grille[cX][j] % 10;
            grille[cX][j] = 0; progression[coul]++;
        }
    }
    for (int i = 0; i < LIGNES; i++) { // Colonne
        if (grille[i][cY] != 0) {
            int coul = grille[i][cY] % 10;
            grille[i][cY] = 0; progression[coul]++;
        }
    }
}

// 3. EFFET LIGNE DE 6
void effet_supprimer_couleur(int grille[LIGNES][COLONNES], int progression[6], int typePierre) {
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if ((grille[i][j] % 10) == typePierre) { 
                grille[i][j] = 0; progression[typePierre]++;
            }
        }
    }
}

// MOTEUR DE RESOLUTION
int resoudre_plateau(int grille[LIGNES][COLONNES], int progression[6]) {
    int modif = 0;
    int items_a_detruire[LIGNES][COLONNES] = {0}; 
    int creation_special[LIGNES][COLONNES] = {0}; 

    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            int val = grille[i][j];
            if (val == 0) continue;
            
            int typeBase = val % 10; 

            // Scans
            int lenH = 0, k = j;
            while (k < COLONNES && (grille[i][k] % 10) == typeBase) { lenH++; k++; }
            int lenV = 0, l = i;
            while (l < LIGNES && (grille[l][j] % 10) == typeBase) { lenV++; l++; }

            // A. Ligne de 6+
            if (lenH >= 6 || lenV >= 6) {
                effet_supprimer_couleur(grille, progression, typeBase);
                gravite(grille);
                modif_grille(grille);
                resoudre_plateau(grille, progression); 
                return 1;
            }
            // B. Croix de 9
            if (lenH >= 5 && lenV >= 5) {
                effet_croix_explosion(grille, progression, i, j);
                gravite(grille);
                modif_grille(grille);
                resoudre_plateau(grille, progression);
                return 1;
            }
            // C. Ligne de 5 : Création BOMBE
            if (lenH == 5) {
                for(int m=0; m<lenH; m++) items_a_detruire[i][j+m] = 1;
                creation_special[i][j+2] = typeBase + 10; 
                items_a_detruire[i][j+2] = 0; 
                modif = 1;
            }
            else if (lenV == 5) {
                for(int m=0; m<lenV; m++) items_a_detruire[i+m][j] = 1;
                creation_special[i+2][j] = typeBase + 10;
                items_a_detruire[i+2][j] = 0;
                modif = 1;
            }
            // D. Match 3 ou 4
            else {
                if (lenH >= 3) {
                    int a_une_bombe = 0;
                    for (int m = 0; m < lenH; m++) {
                        if (grille[i][j+m] > 10) a_une_bombe = 1;
                        items_a_detruire[i][j+m] = 1;
                    }
                    if (a_une_bombe) {
                        effet_bombe(grille, progression, i, j + 1);
                        gravite(grille);
                        modif_grille(grille);
                        resoudre_plateau(grille, progression);
                        return 1;
                    }
                    modif = 1;
                }
                if (lenV >= 3) {
                    int a_une_bombe = 0;
                    for (int m = 0; m < lenV; m++) {
                        if (grille[i+m][j] > 10) a_une_bombe = 1;
                        items_a_detruire[i+m][j] = 1;
                    }
                    if (a_une_bombe) {
                        effet_bombe(grille, progression, i + 1, j);
                        gravite(grille);
                        modif_grille(grille);
                        resoudre_plateau(grille, progression);
                        return 1;
                    }
                    modif = 1;
                }
            }
            // Carré 2x2
            if (i < LIGNES-1 && j < COLONNES-1) {
                if ((grille[i][j]%10) == typeBase && (grille[i+1][j]%10) == typeBase &&
                    (grille[i][j+1]%10) == typeBase && (grille[i+1][j+1]%10) == typeBase) {
                        items_a_detruire[i][j] = 1; items_a_detruire[i+1][j] = 1;
                        items_a_detruire[i][j+1] = 1; items_a_detruire[i+1][j+1] = 1;
                        modif = 1;
                }
            }
        }
    }

    if (modif) {
        for (int i = 0; i < LIGNES; i++) {
            for (int j = 0; j < COLONNES; j++) {
                if (items_a_detruire[i][j]) {
                    int c = grille[i][j] % 10;
                    if (c != 0) progression[c]++;
                    grille[i][j] = 0;
                }
            }
        }
        for (int i = 0; i < LIGNES; i++) {
            for (int j = 0; j < COLONNES; j++) {
                if (creation_special[i][j] != 0) grille[i][j] = creation_special[i][j];
            }
        }
        gravite(grille);
        modif_grille(grille);
        resoudre_plateau(grille, progression); 
    }
    return modif;
}

// ==========================================
// PARTIE 2 : OBJECTIFS (3 ou 4 PIERRES)
// ==========================================

void genererObjectifs(int niveau, int objectifs[6]) {
    for(int k=0; k<6; k++) objectifs[k] = 0;
    
    int total_cases = LIGNES * COLONNES;
    
    // Difficulté progressive
    float ratio = 0.15 + (niveau * 0.05); // 15% puis augmente
    if(ratio > 0.8) ratio = 0.8;
    int total_a_vider = total_cases * ratio;
    if (total_a_vider < 6) total_a_vider = 6;

    // Nombre d'objectifs (couleurs différentes)
    int nb_objectifs = 2; // Niveau 1-2
    if (niveau >= 3) nb_objectifs = 3; // Niveau 3-6 (3 Pierres)
    if (niveau >= 7) nb_objectifs = 4; // Niveau 7+ (4 Pierres)

    int couleurs_choisies[6] = {0};
    int count = 0;

    while(count < nb_objectifs) {
        int c = (rand() % 5) + 1;
        if(couleurs_choisies[c] == 0) {
            couleurs_choisies[c] = 1;
            objectifs[c] = total_a_vider / nb_objectifs;
            if(objectifs[c] < 1) objectifs[c] = 1;
            count++;
        }
    }
}

int verif_victoire(int objectifs[6], int progression[6]) {
    for (int i = 1; i <= 5; i++) {
        if (progression[i] < objectifs[i]) return 0; 
    }
    return 1; 
}

// ==========================================
// PARTIE 3 : AFFICHAGE & HUD
// ==========================================

void afficherInfosHUD(char pseudo[], int niveau, int coups, int vies, int objectifs[6], int progression[6], int tempsRestant) {
    text_color(WHITE);
    printf("====================================================\n"); // Ligne raccourcie
    printf(" JOUEUR: %s | NIV: %d | VIES: %d | COUPS: %d | TEMPS: ", pseudo, niveau, vies, coups);
    
    if (tempsRestant <= 10) text_color(LIGHTRED);
    else text_color(LIGHTGREEN);
    printf("%ds", tempsRestant);
    text_color(WHITE); 
    
    printf("\n====================================================\n"); // Ligne raccourcie
    printf(" CONTRAT (Pierres a collecter) :\n");
    
    int a_un_objectif = 0;
    for (int i = 1; i <= 5; i++) {
        if (objectifs[i] > 0) {
            text_color(CODE_COULEURS[i]);
            printf("  %s", NOM_PIERRES[i]); // Utilise NOM_PIERRES (Emeraude, etc.)
            text_color(WHITE);
            printf(" : %d / %d", progression[i], objectifs[i]);
            if (progression[i] >= objectifs[i]) {
                text_color(LIGHTGREEN); printf(" (OK!)");
            }
            printf("\n");
            a_un_objectif = 1;
        }
    }
    text_color(WHITE);
    if(!a_un_objectif) printf("  (Aucun objectif)\n");
    printf("----------------------------------------------------\n");
    printf(" [Z/Q/S/D] Deplacer | [ESPACE] Selection | [P] Pause\n");
}

void afficherPlateau(int grille[LIGNES][COLONNES], int cX, int cY, int selActive, int selX, int selY) {
    printf("\n");
    for (int i = 0; i < LIGNES; i++) {
        printf("   "); 
        for (int j = 0; j < COLONNES; j++) {
            
            int valeur = grille[i][j];
            int type = valeur % 10;       
            int estSpecial = (valeur > 10); 

            int couleur = CODE_COULEURS[type];
            
            int estCurseur = (i == cX && j == cY);
            int estSelectionne = (selActive && i == selX && j == selY);

            // Curseur
            text_color(WHITE);
            if (estSelectionne && estCurseur) printf("<");
            else if (estCurseur) printf("[");
            else if (estSelectionne) printf("*");
            else printf(" "); 

            // Chiffre
            if (estSpecial) {
                bg_color(couleur); 
                text_color(WHITE);
                printf("%d", type); 
            } 
            else {
                bg_color(BLACK);
                text_color(couleur);
                printf("%d", type);
            }

            bg_color(BLACK); 
            text_color(WHITE);
            if (estSelectionne && estCurseur) printf(">");
            else if (estCurseur) printf("]");
            else if (estSelectionne) printf("*");
            else printf(" "); 
        }
        printf("\n"); // Enlevé le double \n pour réduire la hauteur et éviter le scrolling
    }
    text_color(WHITE);
}

// ==========================================
// PARTIE 4 : BOUCLE DE JEU
// ==========================================

int lancerNiveau(char pseudo[], int niveau, int *vies) {
    int grille[LIGNES][COLONNES];
    int total_cases = LIGNES * COLONNES;

    int coups_max = 20 + (total_cases / 40) + (niveau * 2);
    if (coups_max > 80) coups_max = 80; 
    int coups = coups_max;

    int temps_total = 60 + (total_cases / 10);
    if (temps_total > 300) temps_total = 300; 

    int objectifs[6] = {0};
    int progression[6] = {0};
    
    creation_grille(grille);
    int poubelle[6] = {0}; 
    resoudre_plateau(grille, poubelle); 

    genererObjectifs(niveau, objectifs);

    int cX = LIGNES/2, cY = COLONNES/2; 
    int selActive = 0, sX = -1, sY = -1;
    int running = 1;

    time_t debut = time(NULL);
    int tempsRestant = temps_total;
    int dernierTempsAffiche = temps_total;
    int besoinRafraichissement = 1;

    hide_cursor();

    while (running && coups > 0 && tempsRestant > 0) {
        
        int ecoule = (int)difftime(time(NULL), debut);
        tempsRestant = temps_total - ecoule;

        if (tempsRestant != dernierTempsAffiche) {
            dernierTempsAffiche = tempsRestant;
            besoinRafraichissement = 1;
        }

        if (verif_victoire(objectifs, progression)) {
            clrscr(); 
            if(tempsRestant < 0) tempsRestant = 0;
            afficherInfosHUD(pseudo, niveau, coups, *vies, objectifs, progression, tempsRestant);
            afficherPlateau(grille, cX, cY, selActive, sX, sY);
            
            text_color(LIGHTGREEN);
            printf("\n\n*** VICTOIRE ! Niveau termine ! ***\n");
            text_color(WHITE);
            show_cursor();
            system("pause");
            return 1;
        }

        if (besoinRafraichissement) {
            // FIX POUR L'AFFICHAGE QUI SCROLLE :
            // Si clrscr() ne marche pas, essayez de décommenter la ligne suivante :
            // system("cls"); 
            clrscr();
            
            if(tempsRestant < 0) tempsRestant = 0;
            afficherInfosHUD(pseudo, niveau, coups, *vies, objectifs, progression, tempsRestant);
            afficherPlateau(grille, cX, cY, selActive, sX, sY);
            besoinRafraichissement = 0;
        }

        if (kbhit()) { 
            char touche = getch(); 
            besoinRafraichissement = 1; 

            switch (touche) {
                case 'p': show_cursor(); return 0; 
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
                                tmp = grille[cX][cY]; grille[cX][cY] = grille[sX][sY]; grille[sX][sY] = tmp;
                                selActive = 0;
                            }
                        } else {
                            selActive = 0;
                        }
                    }
                    break;
            }
        } else {
            Sleep(50); 
        }
    }

    show_cursor();
    printf("\n\n");
    text_color(LIGHTRED);
    if (tempsRestant <= 0) printf("DEFAITE... Temps ecoule !\n");
    else printf("DEFAITE... Plus de coups disponibles.\n");
    
    text_color(WHITE);
    system("pause");
    return 0; 
}

// ==========================================
// PARTIE 5 : MENU
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
    printf("5. EXTENSIONS & BONUS :\n");
    printf("   - Ligne de 5 : Cree une BOMBE (Fond colore).\n");
    printf("   - Bombe + 2 items : EXPLOSION DE ZONE (Carre).\n");
    printf("   - Ligne de 6 : Detruit toute la couleur.\n");
    printf("   - Croix (5+5) : Explosion Ligne + Colonne.\n\n");
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

    SetConsoleTitle("ECE HEROES - Match 3");

    while (continuerMenu) {
        clrscr();
        text_color(YELLOW);
        printf("--- ECE HEROES : MENU PRINCIPAL ---\n");
        text_color(WHITE);
        printf("1. Regles\n2. Nouvelle Partie\n3. Charger\n4. Quitter\n\nChoix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: afficherRegles(); break;
            case 2:
                printf("Pseudo : "); scanf("%s", pseudo);
                vies = VIES_INIT; niveau = 1; continuerMenu = 0; 
                break;
            case 3:
                printf("Pseudo : "); scanf("%s", pseudo);
                if (chargerSauvegarde(pseudo, &niveau, &vies)) continuerMenu = 0;
                else { printf("Introuvable.\n"); system("pause"); }
                break;
            case 4: return 0;
            default: break;
        }
    }

    while (vies > 0) {
        if (lancerNiveau(pseudo, niveau, &vies)) {
            clrscr();
            printf("Sauvegarder ? (1=Oui/0=Non) : ");
            int sv; scanf("%d", &sv);
            if (sv == 1) sauvegarderPartie(pseudo, niveau + 1, vies);
            niveau++;
        } else {
            vies--;
            if (vies > 0) {
                clrscr();
                text_color(LIGHTRED);
                printf("Vies restantes : %d\nRecommencer ? (1=Oui/0=Non) : ", vies);
                int retry; scanf("%d", &retry);
                if (!retry) break;
            }
        }
    }

    printf("\nGAME OVER\n");
    system("pause");
    return 0;
}