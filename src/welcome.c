#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h> // Pour charger les images
#include <stdbool.h>
#include "barre_nav.h"
#include "welcome.h"
#include "search_bar.h"
#include "render_text.h"
#include "calendar.h"
#include "création.h"

// Structure pour un bouton
typedef struct {
    SDL_Rect rect;          // Zone du bouton
    SDL_Texture *texture;   // Texture du bouton
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
// Fonction pour charger une texture à partir d'un fichier
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        SDL_Log("Erreur de chargement de l'image %s: %s", filePath, IMG_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Fonction principale pour afficher l'écran de bienvenue
void render_welcome(SDL_Renderer* renderer, TTF_Font* font, bool* running, int window_width, int window_height, AppState* currentState) {
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color whiteColor = {255, 255, 255, 255};

    // Charger les images de fond et des boutons
    SDL_Texture* backgroundTexture = loadTexture(renderer, "background_welcome.png");
    SDL_Texture* calendarTexture = loadTexture(renderer, "background_calendar.jpg");
    SDL_Texture* creationTexture = loadTexture(renderer, "background_creation.png");
    SDL_Texture* tenuesTexture = loadTexture(renderer, "background_tenues.png");

    if (!backgroundTexture || !calendarTexture || !creationTexture || !tenuesTexture) {
        SDL_Log("Erreur de chargement des textures");
        return;
    }

    // Dessin principal
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fond blanc opaque
    SDL_RenderClear(renderer);

    // Afficher l'image de fond en dessous de la barre de recherche
    SDL_Rect backgroundRect = {0, 50, window_width, window_height - 50}; // L'image de fond commencera sous la barre de recherche
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

    // Dessiner la barre de recherche
    renderSearchBar(renderer, font, window_width, whiteColor, currentState);

    // Définir les rectangles pour le texte de bienvenue
    SDL_Rect welcomeTextRect = {0, 100, window_width, 50}; // Le texte "Bienvenue Kassandre"
    SDL_Rect questionTextRect = {0, 150, window_width, 50}; // Le texte "Qu'est-ce que tu veux faire ?"

    // Afficher le texte centré
    renderCenteredText(renderer, font, "Bienvenue Kassandre", welcomeTextRect, black);
    renderCenteredText(renderer, font, "Que souhaitez-vous faire ?", questionTextRect, black);

    // Définir les boutons (ajustés pour 150x150)
    int button_size = 150; // Taille des boutons
    Button buttons[] = {
        {{(window_width - button_size) / 2, 250, button_size, button_size}, calendarTexture}, // Bouton au centre
        {{(window_width / 2) - button_size - 50, 450, button_size, button_size}, creationTexture}, // Bouton à gauche
        {{(window_width / 2) + 50, 450, button_size, button_size}, tenuesTexture} // Bouton à droite
    };

    // Dessiner les boutons
    for (int i = 0; i < 3; i++) {
        if (buttons[i].texture) {
            SDL_RenderCopy(renderer, buttons[i].texture, NULL, &buttons[i].rect); // Dessiner la texture
        }
    }

    // Gestion des événements
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
                *currentState = STATE_CALENDAR;
            } else if (x >= buttons[1].rect.x && x <= buttons[1].rect.x + buttons[1].rect.w &&
                       y >= buttons[1].rect.y && y <= buttons[1].rect.y + buttons[1].rect.h) {
                *currentState = STATE_CREATION;
            } else if (x >= buttons[2].rect.x && x <= buttons[2].rect.x + buttons[2].rect.w &&
                       y >= buttons[2].rect.y && y <= buttons[2].rect.y + buttons[2].rect.h) {
                *currentState = STATE_TENUES;
            }
        }
    }

    // Présenter le rendu
    SDL_RenderPresent(renderer);

    // Libération des textures
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(calendarTexture);
    SDL_DestroyTexture(creationTexture);
    SDL_DestroyTexture(tenuesTexture);
}


