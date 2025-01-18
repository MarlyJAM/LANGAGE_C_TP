#ifndef CREA_BAR_H
#define CREA_BAR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


// Fonction pour afficher la barre de recherche
void render_crea_bar(SDL_Renderer *renderer, TTF_Font *font, int window_width, SDL_Color color);

#endif // CREA_BAR_H