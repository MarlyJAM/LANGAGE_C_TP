#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "search_bar.h"  // Inclusion du fichier de la barre de recherche

#define WINDOW_WIDTH 480  // Largeur de la fenêtre
#define WINDOW_HEIGHT 800 // Hauteur de la fenêtre

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

int main(int argc, char *argv[]) {
    // Initialisation de SDL et SDL_ttf
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Création de la fenêtre et du renderer
    SDL_Window *window = SDL_CreateWindow("My Dressing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charger la police avec une taille plus petite
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf", 16);
    if (!font) {
        fprintf(stderr, "Erreur chargement de la police: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Couleur pour le texte de la barre de recherche
    SDL_Color whiteColor = {255, 255, 255, 255};

    // Définition des boutons en forme de triangle (centrés pour largeur 480)
    Button buttons[] = {
        {{165, 250, 150, 150}, "CALENDRIER"}, // Bouton en haut (centre)
        {{80, 450, 150, 150}, "CREATION"},    // Bouton en bas à gauche
        {{250, 450, 150, 150}, "TENUES"}      // Bouton en bas à droite
    };

    // Boucle principale
    int running = 1;
    SDL_Event event;
    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Nettoyage de l'écran
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fond blanc
        SDL_RenderClear(renderer);

        // Affichage de la barre de recherche
        renderSearchBar(renderer, font, WINDOW_WIDTH, whiteColor);

        // Affichage du texte de bienvenue
        SDL_Color textColor = {0, 0, 0, 255};
        renderCenteredText(renderer, font, "Bienvenue Kassandre", (SDL_Rect){0, 70, WINDOW_WIDTH, 30}, textColor);
        renderCenteredText(renderer, font, "par quoi commence-t-on aujourd'hui ?", (SDL_Rect){0, 110, WINDOW_WIDTH, 30}, textColor);

        // Affichage des boutons
        SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255); // Couleur violette pour les boutons
        for (int i = 0; i < 3; i++) {
            SDL_RenderFillRect(renderer, &buttons[i].rect); // Dessine le rectangle
            renderCenteredText(renderer, font, buttons[i].label, buttons[i].rect, textColor); // Affiche le texte centré
        }

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
    }

    // Libération des ressources
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
