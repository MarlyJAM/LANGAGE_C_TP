#ifndef WELCOME_H
#define WELCOME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

// Prototypes des fonctions
void renderCenteredText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect rect, SDL_Color color);
// Fonction principale pour afficher l'écran d'accueil
void render_welcome(SDL_Renderer* renderer, TTF_Font* font, bool* running, int window_width, int window_height, AppState* currentState);

#endif





