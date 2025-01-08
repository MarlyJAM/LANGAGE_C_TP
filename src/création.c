#include "création.h"
#include "ajouter_vêtement.h"
#include "render_text.h"
#include "créatenue.h"
#include "nav.h"
#include "graphics.h"
#include "barre_nav.h"
#include "search_bar.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

// Couleur des boutons
SDL_Color buttonColor = {200, 200, 200, 255};  // Gris clair

// Fonction pour dessiner un bouton
void render_button_c(SDL_Renderer *renderer, TTF_Font *font, Button_c *button) {
    // Dessiner le rectangle arrondi
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, 255);
    draw_rounded_rect(renderer, button->rect, 10); // Rayon des coins arrondis = 10

    // Dessiner le texte au centre du bouton
    SDL_Color textColor = {0, 0, 0, 255};  // Couleur noire pour le texte
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, button->text, textColor);
    if (!textSurface) {
        printf("Erreur lors du rendu du texte : %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        printf("Erreur lors de la création de la texture : %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    int textWidth = 0, textHeight = 0;
    TTF_SizeText(font, button->text, &textWidth, &textHeight);

    SDL_Rect textRect = {
        button->rect.x + (button->rect.w - textWidth) / 2,
        button->rect.y + (button->rect.h - textHeight) / 2,
        textWidth,
        textHeight
    };

    // Copier la texture du texte sur le renderer
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Libérer les ressources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// Fonction principale pour afficher la page de création
void render_creation(SDL_Renderer *renderer, TTF_Font *font, bool *running, int window_width, AppState *currentState) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color buttonColor1 = {70, 130, 180, 255}; // Bleu acier
    SDL_Color buttonColor2 = {46, 139, 87, 255}; // Vert forêt

    Button_c button_add_clothes = {
        .rect = {window_width / 2 - BUTTON_WIDTH / 2, 300, BUTTON_WIDTH, BUTTON_HEIGHT},
        .color = buttonColor1,
        .text = "Ajouter un vetement"
    };

    Button_c button_generate_outfit = {
        .rect = {window_width / 2 - BUTTON_WIDTH / 2, 400, BUTTON_WIDTH, BUTTON_HEIGHT},
        .color = buttonColor2,
        .text = "Creer une tenue"
    };

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

            // Vérification des clics
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                // Gestion du clic sur "Ajouter un vêtement"
                if (is_point_in_rect(x, y, &button_add_clothes.rect)) {
                    printf("Bouton 'Ajouter un vêtement' cliqué !\n");
                    ajouter_vetement(currentState);  // Appel à la fonction pour ajouter un vêtement
                }
                // Gestion du clic sur "Créer une tenue"
                else if (is_point_in_rect(x, y, &button_generate_outfit.rect)) {
                    printf("Bouton 'Créer une tenue' cliqué !\n");
                    render_createnue(); // Appel à la fonction pour créer une tenue
                }
            }

            // Gestion de la barre de recherche pour revenir à l'accueil
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;

                // Vérifiez si le clic est dans la zone de la barre de recherche
                if (x >= 0 && x <= window_width && y >= 0 && y <= 50) { // Barre de recherche
                    printf("Retour à l'accueil\n");
                    *currentState = STATE_HOME;  // Passer à la page d'accueil
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

        // Dessiner les boutons
        render_button_c(renderer, font, &button_add_clothes);
        render_button_c(renderer, font, &button_generate_outfit);

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
    }
}

// #include "création.h"
// #include "ajouter_vêtement.h"
// #include "render_text.h"
// #include "créatenue.h"
// #include "nav.h"  
// #include "graphics.h"
// #include "barre_nav.h"
// #include "search_bar.h"
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_ttf.h>
// #include <stdio.h>
// #include <stdbool.h>


// // Couleur des boutons
// SDL_Color buttonColor = {200, 200, 200, 255};  // Gris clair

// // Fonction pour dessiner un bouton
// void render_button_c(SDL_Renderer *renderer, TTF_Font *font, Button_c *button) {
//     // Dessiner le rectangle arrondi
//     SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, 255);
//     draw_rounded_rect(renderer, button->rect, 10); // Rayon des coins arrondis = 10

//     // Dessiner le texte au centre du bouton
//     SDL_Color textColor = {0, 0, 0, 255};  // Couleur noire pour le texte
//     SDL_Surface *textSurface = TTF_RenderText_Solid(font, button->text, textColor);
//     if (!textSurface) {
//         printf("Erreur lors du rendu du texte : %s\n", TTF_GetError());
//         return;
//     }
//     SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
//     if (!textTexture) {
//         printf("Erreur lors de la création de la texture : %s\n", SDL_GetError());
//         SDL_FreeSurface(textSurface);
//         return;
//     }

//     int textWidth = 0, textHeight = 0;
//     TTF_SizeText(font, button->text, &textWidth, &textHeight);

//     SDL_Rect textRect = {
//         button->rect.x + (button->rect.w - textWidth) / 2,
//         button->rect.y + (button->rect.h - textHeight) / 2,
//         textWidth,
//         textHeight
//     };

//     // Copier la texture du texte sur le renderer
//     SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

//     // Libérer les ressources
//     SDL_FreeSurface(textSurface);
//     SDL_DestroyTexture(textTexture);
// }

// // Fonction principale pour afficher la page de création
// void render_creation(SDL_Renderer *renderer, TTF_Font *font, bool *running, int window_width,AppState* currentState) {
//     SDL_Color white = {255, 255, 255, 255};
//     SDL_Color buttonColor1 = {70, 130, 180, 255}; // Bleu acier
//     SDL_Color buttonColor2 = {46, 139, 87, 255}; // Vert forêt

//    Button_c button_add_clothes = {
//     .rect = {window_width / 2 - BUTTON_WIDTH / 2, 300, BUTTON_WIDTH, BUTTON_HEIGHT},
//     .color = buttonColor1,
//     .text = "Ajouter un vetement"
//     };

//     Button_c button_generate_outfit = {
//         .rect = {window_width / 2 - BUTTON_WIDTH / 2, 400, BUTTON_WIDTH, BUTTON_HEIGHT},
//         .color = buttonColor2,
//         .text = "Creer une tenue"
//     };


//     SDL_Event event;
//     bool in_creation = true;
    
//     // Boucle d'affichage de la page de création
//     while (in_creation && *running) {
//         // Gestion des événements
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_QUIT) {
//                 *running = false;
//                 in_creation = false;
//             }

//             if (event.type == SDL_MOUSEBUTTONDOWN) {
//                 int x = event.button.x;
//                 int y = event.button.y;

//                 // Gestion du clic sur "Ajouter un vêtement"
//                 if (is_point_in_rect(x, y, &button_add_clothes.rect)) {
//                     printf("Bouton 'Ajouter un vêtement' cliqué !\n");
//                     ajouter_vetement();  // Appel à la fonction pour ajouter un vêtement
//                 }
//                 // Gestion du clic sur "Créer une tenue"
//                 else if (is_point_in_rect(x, y, &button_generate_outfit.rect)) {
//                     printf("Bouton 'Créer une tenue' cliqué !\n");
//                     // Appel à la fonction pour créer une tenue
//                     render_createnue();
                    
//                 }
//             }
//         }
//     }
// // Effacer l'écran avec un fond blanc
//         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//         SDL_RenderClear(renderer);
       
//         renderSearchBar(renderer, font, WINDOW_WTH, white,currentState);
//         printf("Bouton recherche");
//         // Dessiner la barre de navigation
//         render_navigation(renderer, font, window_width);

//         // Dessiner les boutons
//         render_button_c(renderer, font, &button_add_clothes);
//         render_button_c(renderer, font, &button_generate_outfit);

//         // Mettre à jour l'affichage
//         SDL_RenderPresent(renderer);
// }
