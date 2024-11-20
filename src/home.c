#include "home.h"
#include "render_text.h"  // Inclure la déclaration de render_text

void render_home(SDL_Renderer* renderer, TTF_Font* font, bool* change_to_welcome, bool* running) {
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color purple = { 128, 0, 128, 255 };
    SDL_Color border_color = { 255, 215, 0, 255 };

    // Effacer l'écran avec une couleur de fond violette
    SDL_SetRenderDrawColor(renderer, 75, 0, 130, 255);
    SDL_RenderClear(renderer);

    // Dessiner le bouton "COMMENCER"
    SDL_Rect button_rect = { 140, 50, 200, 60 };
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
    SDL_RenderFillRect(renderer, &button_rect);
    render_text(renderer, font, "COMMENCER", 150, 60, purple);

    // Dessiner le texte "MY DRESSING"
    SDL_SetRenderDrawColor(renderer, purple.r, purple.g, purple.b, purple.a);
    SDL_Rect text_rect = { 90, 300, 300, 100 };
    SDL_RenderFillRect(renderer, &text_rect);
    render_text(renderer, font, "MY DRESSING", 140, 320, white);

    // Gestion des événements
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = false;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x = event.button.x;
            int y = event.button.y;
            if (x >= button_rect.x && x <= (button_rect.x + button_rect.w) &&
                y >= button_rect.y && y <= (button_rect.y + button_rect.h)) {
                *change_to_welcome = true;
            }
        }
    }

    SDL_RenderPresent(renderer);
}
