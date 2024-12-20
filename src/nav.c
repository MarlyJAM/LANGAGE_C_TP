#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "nav.h"
#include "render_text.h"  

#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 50

void render_navigation(SDL_Renderer *renderer, TTF_Font *font, int window_width) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color purple = {128, 0, 128, 255};

    int button_margin = 20;
    int button_y = 700;  // Position verticale des boutons (proche du bas de l'écran)

    // Dessiner le bouton "Calendrier"
    int button_x1 = window_width / 4 - BUTTON_WIDTH / 2;
    SDL_Rect button1_rect = {button_x1, button_y, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_SetRenderDrawColor(renderer, purple.r, purple.g, purple.b, purple.a);
    SDL_RenderFillRect(renderer, &button1_rect);
    render_text(renderer, font, "Calendrier", button_x1 + 10, button_y + 10, white);

    // Dessiner le bouton "Tenues"
    int button_x2 = window_width / 2 - BUTTON_WIDTH / 2;
    SDL_Rect button2_rect = {button_x2, button_y, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_SetRenderDrawColor(renderer, purple.r, purple.g, purple.b, purple.a);
    SDL_RenderFillRect(renderer, &button2_rect);
    render_text(renderer, font, "Tenues", button_x2 + 10, button_y + 10, white);

    // Dessiner le bouton "Création"
    int button_x3 = 3 * window_width / 4 - BUTTON_WIDTH / 2;
    SDL_Rect button3_rect = {button_x3, button_y, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_SetRenderDrawColor(renderer, purple.r, purple.g, purple.b, purple.a);
    SDL_RenderFillRect(renderer, &button3_rect);
    render_text(renderer, font, "Création", button_x3 + 10, button_y + 10, white);
}
