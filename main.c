#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "config.h"
#include "jeu.h"
#include "affichage.h"

// sauvegarde

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

// niveau

int lancerNiveau(char pseudo[], int niveau, int *vies) {
    int grille[LIGNES][COLONNES];
    int objectifs[6] = {0};
    int progression[6] = {0};
    int total_cases = LIGNES * COLONNES;
    
    int cX = LIGNES / 2;
    int cY = COLONNES / 2;
    int selActive = 0;
    int sX = -1, sY = -1;
    int running = 1;

    // difficulte
    int coups_max = 20 + (total_cases / 40) + (niveau * 2);
    if (coups_max > 80) coups_max = 80;
    int coups = coups_max;

    int temps_total = 60 + (total_cases / 10);
    if (temps_total > 300) temps_total = 300;

    // init
    creation_grille(grille);
    
    // clean debut
    int poubelle[6] = {0};
    resoudre_plateau(grille, poubelle); 

    genererObjectifs(niveau, objectifs);

    time_t debut = time(NULL);
    int tempsRestant = temps_total;
    int dernierTempsAffiche = temps_total;
    int besoinRafraichissement = 1;

    hide_cursor(); 
    clrscr(); 

    while (running && coups > 0 && tempsRestant > 0) {

        // timer
        int ecoule = (int)difftime(time(NULL), debut);
        tempsRestant = temps_total - ecoule;

        if (tempsRestant != dernierTempsAffiche) {
            dernierTempsAffiche = tempsRestant;
            besoinRafraichissement = 1;
        }

        // verif win
        if (verif_victoire(objectifs, progression)) {
            clrscr();
            if (tempsRestant < 0) tempsRestant = 0;
            
            afficherInfosHUD(pseudo, niveau, coups, *vies, objectifs, progression, tempsRestant);
            afficherPlateau(grille, cX, cY, selActive, sX, sY);
            
            text_color(LIGHTGREEN);
            printf("\n\n*** VICTOIRE ! Niveau termine ! ***\n");
            text_color(WHITE);
            show_cursor();
            system("pause");
            return 1;
        }

        // affichage
        if (besoinRafraichissement) {
            hide_cursor();
            clrscr();
            if (tempsRestant < 0) tempsRestant = 0;
            
            afficherInfosHUD(pseudo, niveau, coups, *vies, objectifs, progression, tempsRestant);
            afficherPlateau(grille, cX, cY, selActive, sX, sY);
            
            besoinRafraichissement = 0;
        }

        // inputs
        if (kbhit()) {
            char touche = getch();
            besoinRafraichissement = 1;

            switch (touche) {
                case 'p': // pause
                    show_cursor();
                    return 0; 
                
                case 'z': if (cX > 0) cX--; break;
                case 's': if (cX < LIGNES - 1) cX++; break;
                case 'q': if (cY > 0) cY--; break;
                case 'd': if (cY < COLONNES - 1) cY++; break;

                case ' ':
                    if (!selActive) {
                        // select
                        selActive = 1; 
                        sX = cX; sY = cY;
                    } else {
                        // deplacement
                        int dist = abs(cX - sX) + abs(cY - sY);
                        
                        if (dist == 1) { 
                            // swap
                            int tmp = grille[cX][cY]; 
                            grille[cX][cY] = grille[sX][sY]; 
                            grille[sX][sY] = tmp;

                            // resolution
                            int combo = resoudre_plateau(grille, progression);

                            if (combo) {
                                coups--; 
                                selActive = 0;
                            } else {
                                // fail, retour
                                tmp = grille[cX][cY]; 
                                grille[cX][cY] = grille[sX][sY]; 
                                grille[sX][sY] = tmp;
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

    // perdu
    show_cursor();
    printf("\n\n");
    text_color(LIGHTRED);
    if (tempsRestant <= 0) printf("DEFAITE... Temps ecoule !\n");
    else printf("DEFAITE... Plus de coups disponibles.\n");
    
    text_color(WHITE);
    system("pause");
    return 0; 
}

// main

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
        printf("1. Lire les regles\n2. Nouvelle Partie\n3. Charger\n4. Quitter\n\nChoix : ");
        
        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            continue;
        }

        switch (choix) {
            case 1: 
                afficherRegles(); 
                break;
            case 2:
                printf("Pseudo : "); 
                scanf("%s", pseudo);
                vies = VIES_INIT; niveau = 1; continuerMenu = 0; 
                break;
            case 3:
                printf("Pseudo : "); 
                scanf("%s", pseudo);
                if (chargerSauvegarde(pseudo, &niveau, &vies)) {
                    continuerMenu = 0;
                } else { 
                    printf("Introuvable.\n"); 
                    system("pause"); 
                }
                break;
            case 4: 
                return 0;
            default: 
                break;
        }
    }

    while (vies > 0) {
        if (lancerNiveau(pseudo, niveau, &vies)) {
            // gagne
            clrscr();
            printf("Sauvegarder ? (1=Oui/0=Non) : ");
            int sv; 
            scanf("%d", &sv);
            if (sv == 1) sauvegarderPartie(pseudo, niveau + 1, vies);
            
            niveau++; 
        } else {
            // perdu
            vies--;
            if (vies > 0) {
                clrscr();
                text_color(LIGHTRED);
                printf("Vies restantes : %d\nRecommencer ? (1=Oui/0=Non) : ", vies);
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