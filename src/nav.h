#ifndef NAV_H
#define NAV_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Fonction pour dessiner la barre de navigation en bas de l'écran
void render_navigation(SDL_Renderer *renderer, TTF_Font *font, int window_width);

#endif
