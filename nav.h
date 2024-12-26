#ifndef NAV_H
#define NAV_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "barre_nav.h"

// Fonction pour dessiner la barre de navigation en bas de l'écran
void render_navigation(SDL_Renderer *renderer, TTF_Font *font, int window_width)  ;
// Fonction pour gérer les clics sur la barre de navigation
void handle_navigation_click(int x, int y, int window_width, AppState *currentState);
bool is_point_in_rect(int x, int y, SDL_Rect *rect);
#endif
