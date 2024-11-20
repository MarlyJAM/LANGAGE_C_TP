#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "création.h"


int main(int argc, char *argv[]) {
    // Initialiser SDL2 (vidéo et gestion des événements)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "Erreur initialisation SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Initialiser SDL_ttf pour gérer les polices
    if (TTF_Init() != 0) {
        fprintf(stderr, "Erreur initialisation TTF: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Créer une fenêtre de taille fixe
    SDL_Window *window = SDL_CreateWindow(
        "Fenêtre avec boutons",                // Titre de la fenêtre
        SDL_WINDOWPOS_CENTERED,                // Position X (centrée)
        SDL_WINDOWPOS_CENTERED,                // Position Y (centrée)
        480,                          // Largeur de la fenêtre
        800,                         // Hauteur de la fenêtre
        SDL_WINDOW_SHOWN                       // La fenêtre est visible
    );
    if (!window) {
        fprintf(stderr, "Erreur création fenêtre: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Créer un rendu pour dessiner dans la fenêtre
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        fprintf(stderr, "Erreur création renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Charger une police avec une taille de 36 points
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/msttcorefonts/Times_New_Roman.ttf", 24);
    if (!font) {
        fprintf(stderr, "Erreur chargement police: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Couleur des boutons
    SDL_Color buttonColor = {200, 200, 200, 255}; // Gris clair

    // Initialiser le premier bouton
    Button button1 = {
        .rect = {480/ 2 - BUTTON_WIDTH / 2, 200, BUTTON_WIDTH, BUTTON_HEIGHT},
        .color = buttonColor,
        .text = "Ajouter un vêtement"
    };

    // Initialiser le deuxième bouton
    Button button2 = {
        .rect = {480/ 2 - BUTTON_WIDTH / 2, 200 + BUTTON_HEIGHT + BUTTON_MARGIN, BUTTON_WIDTH, BUTTON_HEIGHT},
        .color = buttonColor,
        .text = "Création de tenue"
    };

    // Boucle principale
    bool running = true;      // Flag pour maintenir le programme actif
    SDL_Event event;          // Variable pour gérer les événements

    while (running) {
        // Récupérer les événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {  // Quitter si la fenêtre est fermée
                running = false;
            }

            // Gestion des clics de souris
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;  // Position X du clic
                int y = event.button.y;  // Position Y du clic

                if (is_point_in_rect(x, y, &button1.rect)) {
                    printf("Bouton 'Ajouter un vêtement' cliqué !\n");
                } else if (is_point_in_rect(x, y, &button2.rect)) {
                    printf("Bouton 'Création de tenue' cliqué !\n");
                }
            }
        }

        // Effacer l'écran et dessiner un fond blanc
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Couleur blanche
        SDL_RenderClear(renderer);

        // Dessiner les boutons
        render_button(renderer, font, &button1);
        render_button(renderer, font, &button2);

        // Afficher le rendu
        SDL_RenderPresent(renderer);
    }

    // Libérer les ressources
    TTF_CloseFont(font);          // Fermer la police
    SDL_DestroyRenderer(renderer); // Détruire le rendu
    SDL_DestroyWindow(window);    // Détruire la fenêtre
    TTF_Quit();                   // Quitter SDL_ttf
    SDL_Quit();                   // Quitter SDL

    return 0;
}