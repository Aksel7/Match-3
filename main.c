#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "config.h"
#include "jeu.h"
#include "affichage.h"
#include "musique.h"
#include "partie.h"

int main() {
    srand(time(NULL));
    char pseudo[50];
    int niveau = 1;
    int vies = VIES_INIT;
    int choix;
    int continuerMenu = 1;
    int musiqueActive = 1; // par defaut ON

    SetConsoleTitle("ECE HEROES - Match 3");

    while (continuerMenu) {
        clrscr();
        text_color(YELLOW);
        printf("--- ECE HEROES : MENU PRINCIPAL ---\n");
        text_color(WHITE);
        printf("1. Lire les regles\n2. Nouvelle Partie\n3. Charger\n4. Quitter\n");
        
        // affichage etat musique
        if (musiqueActive) printf("5. Musique : [ON]\n");
        else printf("5. Musique : [OFF]\n");
        
        printf("\nChoix : ");
        
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
            case 5:
                musiqueActive = !musiqueActive; // switch on/off
                break;
            default: 
                break;
        }
    }

    while (vies > 0) {
        // on passe l'info musique au niveau
        if (lancerNiveau(pseudo, niveau, &vies, musiqueActive)) {
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