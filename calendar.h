
#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "accessoire.h"

#ifndef CALENDAR_H
#define CALENDAR_H

// Nombre de jours dans un mois (30 pour simplification)
#define DAYS_IN_MONTH 31
#define MAX_TENUES_PER_DAY 10

// Structure pour gérer les tenues par jour
typedef struct {
    char tenues[MAX_TENUES_PER_DAY][MAX_LEN];  // Tableau de tenues pour un jour donné
    int nombre_tenues;  // Nombre de tenues pour ce jour
} JourTenues;

// Déclaration des fonctions

// Fonction pour générer une tenue aléatoire
void generer_tenue(char *tenue) ;
int is_click_on_creation_button(int x, int y, int window_width, int window_height) ;
// Fonction pour afficher le calendrier
void render_calendar(SDL_Renderer *renderer, TTF_Font *font, bool *running, int window_width, int window_height, AppState *currentState);


#endif // CALENDAR_H

