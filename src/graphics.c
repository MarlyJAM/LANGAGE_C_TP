#include "graphics.h"
#include <SDL2/SDL.h>
#include <math.h>

#include <math.h>


// Fonction pour dessiner un rectangle avec des coins arrondis
void draw_rounded_rect(SDL_Renderer* renderer, SDL_Rect rect, int radius) {
    // Dessiner les quatre coins arrondis
    for (int i = 0; i < 4; i++) {
        int corner_x = rect.x + (i % 2) * (rect.w - 2 * radius);
        int corner_y = rect.y + (i / 2) * (rect.h - 2 * radius);
        int cx = corner_x + radius;
        int cy = corner_y + radius;

        for (int angle = 0; angle < 90; angle++) {
            float rad = angle * M_PI / 180;
            int dx = radius * cos(rad);
            int dy = radius * sin(rad);
            SDL_RenderDrawPoint(renderer, cx + dx, cy + dy); // Coin supérieur gauche
            SDL_RenderDrawPoint(renderer, cx - dx, cy + dy); // Coin supérieur droit
            SDL_RenderDrawPoint(renderer, cx + dx, cy - dy); // Coin inférieur gauche
            SDL_RenderDrawPoint(renderer, cx - dx, cy - dy); // Coin inférieur droit
        }
    }

    // Dessiner les bords verticaux et horizontaux du rectangle
    SDL_RenderDrawLine(renderer, rect.x + radius, rect.y, rect.x + rect.w - radius, rect.y); // Haut
    SDL_RenderDrawLine(renderer, rect.x + radius, rect.y + rect.h, rect.x + rect.w - radius, rect.y + rect.h); // Bas
    SDL_RenderDrawLine(renderer, rect.x, rect.y + radius, rect.x, rect.y + rect.h - radius); // Gauche
    SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y + radius, rect.x + rect.w, rect.y + rect.h - radius); // Droite
}
