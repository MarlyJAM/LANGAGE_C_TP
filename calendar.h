#ifndef CALENDAR_H
#define CALENDAR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define MAX_LEN 100
#define MAX_TENUES_PER_DAY 10
#define DAYS_IN_MONTH 30

// Structure pour stocker les tenues par jour
typedef struct {
    char tenues[MAX_TENUES_PER_DAY][MAX_LEN];
    int nombre_tenues;
} JourTenues;

// Déclaration de la fonction
void render_calendar(SDL_Renderer *renderer, TTF_Font *font, bool *running);

#endif

