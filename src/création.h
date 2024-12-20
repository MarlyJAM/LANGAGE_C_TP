
#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifndef CREATION_H
#define CREATION_H

#define WINDOW_WIDTH 480      // Largeur de la fenêtre
#define WINDOW_HEIGHT 800     // Hauteur de la fenêtre
#define BUTTON_WIDTH 200      // Largeur des boutons
#define BUTTON_HEIGHT 60      // Hauteur des boutons
#define BUTTON_MARGIN 40      // Marge entre les boutons


typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    const char *text;
} Button_c;

void render_creation(SDL_Renderer *renderer, TTF_Font *font, bool *running);
bool is_point_in_rect(int x, int y, SDL_Rect *rect) ;

void render_button_c(SDL_Renderer *renderer, TTF_Font *font, Button_c *button); 

#endif // CREATION_H