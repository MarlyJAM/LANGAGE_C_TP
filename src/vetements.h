#ifndef VETEMENTS_H
#define VETEMENTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "barre_nav.h"
 // Ajout de cette ligne pour définir bool

// Déclaration de la fonction afficher_vetements
void afficher_vetements(SDL_Renderer *renderer, TTF_Font *font, bool *running, AppState *currentState);

int renderer_vet(SDL_Renderer *renderer);
void charger_vetements_csv(const char *chemin_fichier);

#endif 
// VETEMENTS_H
