#ifndef HOME_H
#define HOME_H
#define M_PI 3.14159265358979323846

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

// Prototypes des fonctions
void render_home(SDL_Renderer* renderer, TTF_Font* font, bool* change_to_welcome, bool* running);
void render_centered_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Rect rect, SDL_Color color);

#endif
