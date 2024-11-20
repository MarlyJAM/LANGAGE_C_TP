#ifndef WELCOME_H
#define WELCOME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

// Prototypes des fonctions
void renderCenteredText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect rect, SDL_Color color);
void render_welcome(SDL_Renderer* renderer, TTF_Font* font, bool* running);

#endif
