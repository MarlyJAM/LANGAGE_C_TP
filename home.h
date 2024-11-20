#ifndef HOME_H
#define HOME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

// Prototypes des fonctions
void render_home(SDL_Renderer* renderer, TTF_Font* font, bool* change_to_welcome, bool* running);

#endif
