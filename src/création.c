#include "création.h"
#include "ajouter_vêtement.h"
#include "render_text.h"
#include "créatenue.h"
#include "welcome.h"
#include "nav.h"
#include "graphics.h"
#include "barre_nav.h"
#include "search_bar.h"
#include "vetements.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

void render_creation(SDL_Renderer* renderer, TTF_Font* font, bool* running, int window_width, AppState* currentState) {
    SDL_Color white = {255, 255, 255, 255};

    // Charger les images des boutons
    SDL_Texture* addClothesTexture = loadTexture(renderer, "ajout_vetement.png");
    SDL_Texture* createOutfitTexture = loadTexture(renderer, "createnue.png");
    SDL_Texture* showClothesTexture = loadTexture(renderer, "voir_vet.png");

    if (!addClothesTexture || !createOutfitTexture || !showClothesTexture) {
        printf("Erreur de chargement des images des boutons.\n");
        return;
    }

    // Dimensions des boutons
    int buttonWidth = 215;
    int buttonHeight = 166;

    // Positions des boutons (ajustées pour être plus hautes)
    SDL_Rect buttonAddClothesRect = { (window_width - buttonWidth) / 2, 100, buttonWidth, buttonHeight };   // Remonté de 100 pixels
    SDL_Rect buttonCreateOutfitRect = { (window_width - buttonWidth) / 2, 300, buttonWidth, buttonHeight };  // Remonté de 100 pixels
    SDL_Rect buttonShowClothesRect = { (window_width - buttonWidth) / 2, 500, buttonWidth, buttonHeight };    // Remonté de 100 pixels

    SDL_Event event;
    bool in_creation = true;

    // Boucle d'affichage de la page de création
    while (in_creation && *running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *running = false;
                in_creation = false;
            }

            // Gestion des clics
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                // Vérifier les clics sur les boutons
                if (x >= buttonAddClothesRect.x && x <= buttonAddClothesRect.x + buttonAddClothesRect.w &&
                    y >= buttonAddClothesRect.y && y <= buttonAddClothesRect.y + buttonAddClothesRect.h) {
                    printf("Bouton 'Ajouter un vêtement' cliqué !\n");
                    ajouter_vetement(renderer, font); // Appel à la fonction pour ajouter un vêtement
                } else if (x >= buttonCreateOutfitRect.x && x <= buttonCreateOutfitRect.x + buttonCreateOutfitRect.w &&
                           y >= buttonCreateOutfitRect.y && y <= buttonCreateOutfitRect.y + buttonCreateOutfitRect.h) {
                    printf("Bouton 'Créer une tenue' cliqué !\n");
                    render_createnue(); // Appel à la fonction pour créer une tenue
                } else if (x >= buttonShowClothesRect.x && x <= buttonShowClothesRect.x + buttonShowClothesRect.w &&
                           y >= buttonShowClothesRect.y && y <= buttonShowClothesRect.y + buttonShowClothesRect.h) {
                    printf("Bouton 'Voir vêtements' cliqué !\n");
                    renderer_vet(renderer); // Appel à la fonction pour voir les vêtements
                }
            }

            // Gestion de la barre de recherche pour revenir à l'accueil
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;

                // Vérifiez si le clic est dans la zone de la barre de recherche
                if (x >= 0 && x <= window_width && y >= 0 && y <= 50) { // Barre de recherche
                    printf("Retour à l'accueil\n");
                    *currentState = STATE_HOME; // Passer à la page d'accueil
                    in_creation = false; // Quitter la page de création
                }
            }
        }

        // Effacer l'écran avec un fond blanc
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Affichage de la barre de recherche
        renderSearchBar(renderer, font, window_width, white, currentState);

        // Dessiner la barre de navigation
        render_navigation(renderer, font, window_width);

        // Dessiner les boutons avec des images
        SDL_RenderCopy(renderer, addClothesTexture, NULL, &buttonAddClothesRect);
        SDL_RenderCopy(renderer, createOutfitTexture, NULL, &buttonCreateOutfitRect);
        SDL_RenderCopy(renderer, showClothesTexture, NULL, &buttonShowClothesRect);

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
    }

    // Libération des textures
    SDL_DestroyTexture(addClothesTexture);
    SDL_DestroyTexture(createOutfitTexture);
    SDL_DestroyTexture(showClothesTexture);
}




