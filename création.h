
#include <stdbool.h>
#include <stdio.h>
#include "barre_nav.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifndef CREATION_H
#define CREATION_H

#define WINDOW_WIDTH 480      // Largeur de la fenêtre
#define WINDOW_HEIGHT 800     // Hauteur de la fenêtre    
#define BUTTON_MARGIN 40      // Marge entre les boutons
// Définition des constantes pour les dimensions des boutons
#define BUTTON_WIDTH 300
#define BUTTON_HEIGHT 50


typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    const char *text;
} Button_c;

void render_creation(SDL_Renderer *renderer, TTF_Font *font, bool *running, int window_width,AppState* currentState);

void render_button_c(SDL_Renderer *renderer, TTF_Font *font, Button_c *button);

#endif // CREATION_H