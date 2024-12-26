// renderbutton.h
#ifndef RENDERBUTTON_H
#define RENDERBUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void renderbutton(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect button_rect, SDL_Color color);

#endif // RENDERBUTTON_H

