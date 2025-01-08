#include "home.h"
#include "render_text.h"
#include "graphics.h"
#include <SDL2/SDL_image.h>

// Fonction pour afficher le texte centré
void render_centered_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Rect rect, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = { rect.x + (rect.w - surface->w) / 2, rect.y + (rect.h - surface->h) / 2, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Fonction pour afficher l'écran d'accueil avec un fond, "MY DRESSING", et "COMMENCER"
void render_home(SDL_Renderer* renderer, TTF_Font* font, bool* change_to_welcome, bool* running) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color purple = {128, 0, 128, 255};

    // Charger l'image en tant que texture
    SDL_Texture* background = IMG_LoadTexture(renderer, "fond.jpeg");
    if (!background) {
        printf("Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        *running = false;
        return;
    }

    // Définir un rectangle pour l'image de fond
    SDL_Rect window_rect = {0, 0, 640, 480}; 

    // Boucle principale pour l'écran d'accueil
    SDL_Event event;
    while (*running && !(*change_to_welcome)) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                // Vérification du clic sur le bouton "COMMENCER"
                SDL_Rect button_rect = {140, 50, 200, 60};
                if (x >= button_rect.x && x <= (button_rect.x + button_rect.w) &&
                    y >= button_rect.y && y <= (button_rect.y + button_rect.h)) {
                    *change_to_welcome = true;
                }
            }
        }

        // Afficher l'image de fond
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, &window_rect);

        // Dessiner un rectangle arrondi pour le bouton "COMMENCER"
        SDL_Rect button_rect = {140, 50, 200, 60};
        SDL_SetRenderDrawColor(renderer, purple.r, purple.g, purple.b, purple.a);
        draw_rounded_rect(renderer, button_rect, 20);

        // Afficher le texte "COMMENCER"
        render_centered_text(renderer, font, "COMMENCER", button_rect, white);

        // Dessiner un rectangle arrondi pour le texte "MY DRESSING"
        SDL_Rect text_rect = {90, 300, 300, 100};
        SDL_SetRenderDrawColor(renderer, purple.r, purple.g, purple.b, purple.a);
        draw_rounded_rect(renderer, text_rect, 20);

        // Afficher le texte "MY DRESSING"
        render_centered_text(renderer, font, "MY DRESSING", text_rect, white);

        // Afficher le rendu
        SDL_RenderPresent(renderer);
    }

    // Nettoyage de la texture
    SDL_DestroyTexture(background);
}
