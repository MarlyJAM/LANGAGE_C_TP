#include "création.h"
#include "ajouter_vêtement.h"  
#include "render_text.h"       
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>


// Couleur des boutons
SDL_Color buttonColor = {200, 200, 200, 255}; // Gris clair

void render_creation(SDL_Renderer *renderer, TTF_Font *font, bool *running) {

    Button_c button_add_clothes = {
        .rect = {480 / 2 - BUTTON_WIDTH / 2, 300, BUTTON_WIDTH, BUTTON_HEIGHT}, // Bouton centré en haut
        .color = buttonColor,
        .text = "Ajouter un vêtement"
    };

    Button_c button_generate_outfit = {
        .rect = {480 / 2 - BUTTON_WIDTH / 2, 400, BUTTON_WIDTH, BUTTON_HEIGHT}, // Bouton centré au milieu
        .color = buttonColor,
        .text = "Créer une tenue"
    };

    SDL_Event event;
    bool in_creation = true;

    while (in_creation && *running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *running = false;
                in_creation = false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                // Gestion du clic sur "Ajouter un vêtement"
                if (is_point_in_rect(x, y, &button_add_clothes.rect)) {
                    printf("Bouton 'Ajouter un vêtement' cliqué !\n");
                    ajouter_vetement(); // Appel à la fonctionnalité ajouter_vetement
                }
                // Gestion du clic sur "Créer une tenue"
                else if (is_point_in_rect(x, y, &button_generate_outfit.rect)) {
                    printf("Bouton 'Créer une tenue' cliqué !\n");
                    // Appel à la fonction pour créer une tenue
                }
            }
        }

        // Effacer l'écran avec un fond blanc
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Dessiner les boutons
        
        render_button_c(renderer, font, &button_add_clothes);
        render_button_c(renderer, font, &button_generate_outfit);

        // Afficher le rendu
        SDL_RenderPresent(renderer);
    }
}

void render_button_c(SDL_Renderer *renderer, TTF_Font *font, Button_c *button) {
    // Dessiner le rectangle
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, 255);
    SDL_RenderFillRect(renderer, &button->rect);

    // Dessiner le texte
    SDL_Color textColor = {0, 0, 0, 255}; // Noir
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, button->text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    int textWidth = 0, textHeight = 0;
    TTF_SizeText(font, button->text, &textWidth, &textHeight);

    SDL_Rect textRect = {
        button->rect.x + (button->rect.w - textWidth) / 2,
        button->rect.y + (button->rect.h - textHeight) / 2,
        textWidth,
        textHeight
    };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
bool is_point_in_rect(int x, int y, SDL_Rect *rect) {
    return x >= rect->x && x <= rect->x + rect->w && y >= rect->y && y <= rect->y + rect->h;
}