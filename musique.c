#include <windows.h>
#include "musique.h"

// Variable globale pour contrôler la boucle
volatile int musiqueOn = 0;
HANDLE hThreadMusique;

// --- FREQUENCES DES NOTES (Gammes 3, 4 et 5) ---
#define SILENCE 0
#define G3 196
#define A3 220
#define B3 247
#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define Gs4 415
#define A4 440
#define B4 494
#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 784
#define A5 880


// Structure pour une note
typedef struct {
    int freq;
    int duree;
} Note;

// THEME TETRIS 
Note partition[] = {
    {E5, 400}, {B4, 200}, {C5, 200}, {D5, 400}, {C5, 200}, {B4, 200},
    {A4, 400}, {A4, 200}, {C5, 200}, {E5, 400}, {D5, 200}, {C5, 200},
    {B4, 600}, {C5, 200}, {D5, 400}, {E5, 400},
    {C5, 400}, {A4, 400}, {A4, 800},
    
    {SILENCE, 100}, // Petite pause
    
    {D5, 400}, {F5, 200}, {A5, 400}, {G5, 200}, {F5, 200},
    {E5, 600}, {C5, 200}, {E5, 400}, {D5, 200}, {C5, 200},
    {B4, 400}, {B4, 200}, {C5, 200}, {D5, 400}, {E5, 400},
    {C5, 400}, {A4, 400}, {A4, 800},
    
    {SILENCE, 500} // Pause 
};

int taille_partition = sizeof(partition) / sizeof(Note);

DWORD WINAPI ThreadMelodie(LPVOID lpParam) {
    int i = 0;
    while (musiqueOn) {
        // On joue la note
        if (partition[i].freq == SILENCE) {
            Sleep(partition[i].duree);
        } else {
            Beep(partition[i].freq, partition[i].duree);
        }

        i++;
        if (i >= taille_partition) {
            i = 0; 
        }
        // staccato
        Sleep(10); 
    }
    return 0;
}

void lancerMusique() {
    if (musiqueOn) return; // Déjà lancée
    musiqueOn = 1;
    // Lance le thread
    hThreadMusique = CreateThread(NULL, 0, ThreadMelodie, NULL, 0, NULL);
}

void arreterMusique() {
    musiqueOn = 0; // Coupe la boucle
    WaitForSingleObject(hThreadMusique, 200); // Attend un peu l'arrêt
    CloseHandle(hThreadMusique);
}