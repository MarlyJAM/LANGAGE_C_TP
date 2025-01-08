#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "barre_nav.h"
#include "welcome.h"
#include "search_bar.h" // Inclusion de la barre de recherche
#include "render_text.h"  // Inclure la déclaration de render_text
#include "calendar.h"
#include "création.h" //

// Structure pour un bouton
typedef struct {
    SDL_Rect rect;
    const char *label;
} Button;

// Fonction pour afficher du texte centré dans un rectangle
void renderCenteredText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect rect, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {
        rect.x + (rect.w - surface->w) / 2, // Centrer le texte horizontalement
        rect.y + (rect.h - surface->h) / 2, // Centrer le texte verticalement
        surface->w,
        surface->h
    };
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Fonction principale pour afficher l'écran de bienvenue
void render_welcome(SDL_Renderer* renderer, TTF_Font* font, bool* running, int window_width, int window_height, AppState* currentState) {
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color whiteColor = {255, 255, 255, 255}; // Couleur pour la barre de recherche

    // Effacer l'écran avec un fond blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Dessiner la barre de recherche
    renderSearchBar(renderer, font, window_width, whiteColor,currentState);

    // Afficher le texte de bienvenue
    render_text(renderer, font, "Bienvenue Kassandre", 100, 100, black);
    render_text(renderer, font, "Qu'est-ce que tu veux faire?", 55, 150, black);

    // Définition des boutons
    int button_width = 150;
    int button_height = 150;

    Button buttons[] = {
        {{(window_width - button_width) / 2, 250, button_width, button_height}, "CALENDRIER"}, // Bouton au centre
        {{(window_width / 2) - button_width - 50, 450, button_width, button_height}, "CREATION"}, // Bouton à gauche
        {{(window_width / 2) + 50, 450, button_width, button_height}, "TENUES"} // Bouton à droite
    };

    // Affichage des boutons
    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255); // Couleur violette pour les boutons
    for (int i = 0; i < 3; i++) {
        SDL_RenderFillRect(renderer, &buttons[i].rect); // Dessine le rectangle
        renderCenteredText(renderer, font, buttons[i].label, buttons[i].rect, whiteColor); // Affiche le texte centré
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int x = event.button.x;
            int y = event.button.y;

            // Vérifiez si le clic est dans le rectangle des boutons
            if (x >= buttons[0].rect.x && x <= buttons[0].rect.x + buttons[0].rect.w &&
                y >= buttons[0].rect.y && y <= buttons[0].rect.y + buttons[0].rect.h) {
                // Appel de render_calendar avec les paramètres appropriés
                *currentState = STATE_CALENDAR;
            } else if (x >= buttons[1].rect.x && x <= buttons[1].rect.x + buttons[1].rect.w &&
                       y >= buttons[1].rect.y && y <= buttons[1].rect.y + buttons[1].rect.h) {
                // Appel de render_creation
                *currentState = STATE_CREATION;
            } else if (x >= buttons[2].rect.x && x <= buttons[2].rect.x + buttons[2].rect.w &&
                       y >= buttons[2].rect.y && y <= buttons[2].rect.y + buttons[2].rect.h) {
                // Appel de render_tenues
                *currentState = STATE_TENUES;
            }
        }
    }

    SDL_RenderPresent(renderer);
}
