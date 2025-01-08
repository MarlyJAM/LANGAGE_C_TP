#ifndef SEARCH_BAR_H
#define SEARCH_BAR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


// Fonction pour afficher la barre de recherche
void renderSearchBar(SDL_Renderer *renderer, TTF_Font *font, int window_width, SDL_Color color, AppState *currentState);

#endif // SEARCH_BAR_H
