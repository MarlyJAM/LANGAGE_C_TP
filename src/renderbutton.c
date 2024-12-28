#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "graphics.h"
#include "render_text.h"
#include "renderbutton.h"

// Fonction pour dessiner un bouton avec texte centré
void renderbutton(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect button_rect, SDL_Color color) {
    // Dessiner le bouton avec coins arrondis
    int radius = 10;  // Rayon des coins arrondis
    draw_rounded_rect(renderer, button_rect, radius);

    // Afficher le texte centré dans le bouton
    int text_width, text_height;
    TTF_SizeText(font, text, &text_width, &text_height);
    int text_x = button_rect.x + (button_rect.w - text_width) / 2;  // Centrer horizontalement
    int text_y = button_rect.y + (button_rect.h - text_height) / 2;  // Centrer verticalement
    render_text(renderer, font, text, text_x, text_y, (SDL_Color){255, 255, 255, 255});
}