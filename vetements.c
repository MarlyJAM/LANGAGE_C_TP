#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>  // Ajouter pour charger des images (si nécessaire)
#include <stdio.h>
#include <string.h>
#include "vetements.h"
#include "ajouter_vêtement.h"

extern Vetement vetements[MAX_VETEMENTS];
extern int vetement_count;

void afficher_vetements(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Color textColor = {255, 255, 255, 255};  // Couleur du texte (blanc)

    // Effacer l'écran
    SDL_SetRenderDrawColor(renderer, 153, 50, 204, 255);  // Fond violet
    SDL_RenderClear(renderer);

    if (vetement_count == 0) {
        printf("Aucun vêtement à afficher.\n");
    } else {
        printf("Affichage des %d vêtements\n", vetement_count);
    }

    // Parcourir et afficher les informations sur les vêtements
    for (int i = 0; i < vetement_count; i++) {
        // Créer un rectangle autour du vêtement
        SDL_Rect rect = {10, 30 + i * 180, 460, 160};  // Ajuster la taille du rectangle
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Couleur du rectangle (blanc)
        SDL_RenderDrawRect(renderer, &rect);  // Dessiner le rectangle autour du vêtement

        // Afficher l'image du vêtement (si elle existe)
        if (strlen(vetements[i].image_path) > 0) {
            SDL_Surface *imageSurface = IMG_Load(vetements[i].image_path);  // Charger l'image
            if (imageSurface) {
                SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
                SDL_Rect imageRect = {rect.x + 5, rect.y + 5, 50, 50};  // Position et taille de l'image
                SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
                SDL_FreeSurface(imageSurface);
                SDL_DestroyTexture(imageTexture);
            } else {
                printf("Erreur de chargement de l'image : %s\n", SDL_GetError());
            }
        }

        // Afficher les informations du vêtement (Nom, Catégorie, Température, etc.)
        char buffer[200];
        snprintf(buffer, sizeof(buffer), "Nom: %s", vetements[i].nom);
        SDL_Surface *surface = TTF_RenderText_Solid(font, buffer, textColor);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect textRect = {rect.x + 70, rect.y + 10, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Afficher d'autres informations (Catégorie, Température, etc.)
        snprintf(buffer, sizeof(buffer), "Catégorie: %s", vetements[i].categorie);
        surface = TTF_RenderText_Solid(font, buffer, textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        textRect.y += 30;
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        snprintf(buffer, sizeof(buffer), "Température: %s", vetements[i].temperature);
        surface = TTF_RenderText_Solid(font, buffer, textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        textRect.y += 30;
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        snprintf(buffer, sizeof(buffer), "Motif: %s", vetements[i].motif);
        surface = TTF_RenderText_Solid(font, buffer, textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        textRect.y += 30;
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        snprintf(buffer, sizeof(buffer), "Type: %s", vetements[i].type);
        surface = TTF_RenderText_Solid(font, buffer, textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        textRect.y += 30;
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        snprintf(buffer, sizeof(buffer), "Couleur: %s", vetements[i].couleur);
        surface = TTF_RenderText_Solid(font, buffer, textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        textRect.y += 30;
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        snprintf(buffer, sizeof(buffer), "Saison: %s", vetements[i].saison);
        surface = TTF_RenderText_Solid(font, buffer, textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        textRect.y += 30;
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    // Afficher à l'écran
    SDL_RenderPresent(renderer);
}

int renderer_vet() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return -1;
    }

    // Initialisation de SDL_ttf
    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    // Initialiser SDL_image pour charger les images
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erreur IMG_Init : %s\n", IMG_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Créer la fenêtre
    SDL_Window *window = SDL_CreateWindow("My Dressing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 800, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Créer le renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Charger une police pour afficher le texte
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/msttcorefonts/Georgia_Bold_Italic.ttf", 24);
    if (!font) {
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Boucle d'événements pour garder la fenêtre ouverte
    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Appeler la fonction pour afficher les vêtements
        afficher_vetements(renderer, font);
    }

    // Libérer les ressources et quitter SDL
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}

// #include <SDL2/SDL.h> 
// #include <SDL2/SDL_ttf.h>
// #include <stdio.h>
// #include <string.h>
// #include "vetements.h"
// #include "ajouter_vêtement.h"  // Inclure ajouter_vêtement.h pour avoir accès aux données de Vetement

// extern Vetement vetements[MAX_VETEMENTS];  // Déclarer le tableau global de vêtements
// extern int vetement_count;  // Déclarer le compteur des vêtements

// // Fonction pour afficher les vêtements
// void afficher_vetements(SDL_Renderer *renderer, TTF_Font *font) {
//     SDL_Color textColor = {255, 255, 255, 255}; // Couleur du texte (blanc)

//     // Effacer l'écran avant de dessiner (fond noir)
//     SDL_SetRenderDrawColor(renderer,153,50,204,255);
//     SDL_RenderClear(renderer);

//     // Vérifier si vetement_count est supérieur à 0
//     if (vetement_count == 0) {
//         printf("Aucun vêtement à afficher.\n");
//     } else {
//         printf("Affichage des %d vêtements\n", vetement_count); // Afficher le nombre de vêtements
//     }

//     // Parcours de tous les vêtements créés et affichage de leurs informations
//     for (int i = 0; i < vetement_count; i++) {
//         char buffer[200];
//         snprintf(buffer, sizeof(buffer), "Nom: %s", vetements[i].nom);

//         // Afficher le nom du vêtement
//         SDL_Surface *surface = TTF_RenderText_Solid(font, buffer, textColor);
//         if (!surface) {
//             printf("Erreur de création de surface : %s\n", TTF_GetError());
//         }
//         SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
//         if (!texture) {
//             printf("Erreur de création de texture : %s\n", SDL_GetError());
//         }

//         SDL_Rect rect = {10, 30 + i * 180, surface->w, surface->h};  // Espacement plus grand entre chaque vêtement
//         SDL_RenderCopy(renderer, texture, NULL, &rect);

//         SDL_FreeSurface(surface);
//         SDL_DestroyTexture(texture);

//         // Afficher les autres informations sur le vêtement (catégorie, température, etc.)
//         snprintf(buffer, sizeof(buffer), "Categorie: %s", vetements[i].categorie);
//         surface = TTF_RenderText_Solid(font, buffer, textColor);
//         texture = SDL_CreateTextureFromSurface(renderer, surface);
//         rect.y += 30;
//         SDL_RenderCopy(renderer, texture, NULL, &rect);

//         SDL_FreeSurface(surface);
//         SDL_DestroyTexture(texture);

//         snprintf(buffer, sizeof(buffer), "Temperature: %s", vetements[i].temperature);
//         surface = TTF_RenderText_Solid(font, buffer, textColor);
//         texture = SDL_CreateTextureFromSurface(renderer, surface);
//         rect.y += 30;
//         SDL_RenderCopy(renderer, texture, NULL, &rect);

//         SDL_FreeSurface(surface);
//         SDL_DestroyTexture(texture);

//         snprintf(buffer, sizeof(buffer), "Motif: %s", vetements[i].motif);
//         surface = TTF_RenderText_Solid(font, buffer, textColor);
//         texture = SDL_CreateTextureFromSurface(renderer, surface);
//         rect.y += 30;
//         SDL_RenderCopy(renderer, texture, NULL, &rect);

//         SDL_FreeSurface(surface);
//         SDL_DestroyTexture(texture);

//         snprintf(buffer, sizeof(buffer), "Type: %s", vetements[i].type);
//         surface = TTF_RenderText_Solid(font, buffer, textColor);
//         texture = SDL_CreateTextureFromSurface(renderer, surface);
//         rect.y += 30;
//         SDL_RenderCopy(renderer, texture, NULL, &rect);

//         SDL_FreeSurface(surface);
//         SDL_DestroyTexture(texture);

//         snprintf(buffer, sizeof(buffer), "Couleur: %s", vetements[i].couleur);
//         surface = TTF_RenderText_Solid(font, buffer, textColor);
//         texture = SDL_CreateTextureFromSurface(renderer, surface);
//         rect.y += 30;
//         SDL_RenderCopy(renderer, texture, NULL, &rect);

//         SDL_FreeSurface(surface);
//         SDL_DestroyTexture(texture);

//         snprintf(buffer, sizeof(buffer), "Saison: %s", vetements[i].saison);
//         surface = TTF_RenderText_Solid(font, buffer, textColor);
//         texture = SDL_CreateTextureFromSurface(renderer, surface);
//         rect.y += 30;
//         SDL_RenderCopy(renderer, texture, NULL, &rect);

//         SDL_FreeSurface(surface);
//         SDL_DestroyTexture(texture);
//     }

//     // Afficher à l'écran
//     SDL_RenderPresent(renderer);
// }

// // Fonction principale qui gère la fenêtre et les événements
// int renderer_vet() {
//     // Initialisation de SDL
//     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//         printf("Erreur SDL_Init : %s\n", SDL_GetError());
//         return -1;
//     }

//     // Initialisation de SDL_ttf
//     if (TTF_Init() == -1) {
//         printf("Erreur TTF_Init : %s\n", TTF_GetError());
//         SDL_Quit();
//         return -1;
//     }

//     // Créer la fenêtre
//     SDL_Window *window = SDL_CreateWindow("My Dressing",
//                                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//                                           480, 800, SDL_WINDOW_SHOWN);
//     if (!window) {
//         printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
//         TTF_Quit();
//         SDL_Quit();
//         return -1;
//     }

//     // Créer le renderer
//     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     if (!renderer) {
//         printf("Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
//         SDL_DestroyWindow(window);
//         TTF_Quit();
//         SDL_Quit();
//         return -1;
//     }

//     // Charger une police pour afficher le texte
//     TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/msttcorefonts/Georgia_Bold_Italic.ttf", 24); // Assurez-vous d'avoir une police valide
//     if (!font) {
//         printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
//         SDL_DestroyRenderer(renderer);
//         SDL_DestroyWindow(window);
//         TTF_Quit();
//         SDL_Quit();
//         return -1;
//     }

//     // Boucle d'événements pour garder la fenêtre ouverte
//     SDL_Event event;
//     int running = 1;

//     while (running) {
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_QUIT) {
//                 running = 0;
//             }
//         }

//         // Appeler la fonction pour afficher les vêtements
//         afficher_vetements(renderer, font);
//     }

//     // Libérer les ressources et quitter SDL
//     TTF_CloseFont(font);
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     TTF_Quit();
//     SDL_Quit();

//     return 0;
// }

