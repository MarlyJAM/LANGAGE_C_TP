#ifndef ACCESSOIRE_H
#define ACCESSOIRE_H

#define MAX_VETEMENTS 100
#define MAX_LEN 1024
#define MAX_TENUES 100

// Structures
typedef struct {
    char haut[MAX_LEN];
    char pantalon[MAX_LEN];
    char chaussure[MAX_LEN];
    char description[MAX_LEN];
    SDL_Texture *haut_icon;
    SDL_Texture *pantalon_icon;
    SDL_Texture *chaussure_icon;
} Tenue;

extern char hauts[MAX_VETEMENTS][MAX_LEN];
extern char pantalons[MAX_VETEMENTS][MAX_LEN];
extern char chaussures[MAX_VETEMENTS][MAX_LEN];
extern int num_hauts, num_pantalons, num_chaussures;
extern Tenue tenue_affichee;
extern Tenue tenues_sauvegardees[MAX_TENUES];
extern int nombre_tenues_sauvegardees;

#endif
