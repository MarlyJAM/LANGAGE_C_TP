#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vetements.h"
#include "ajouter_vêtement.h"
#include "crea_bar.h"
#include "création.h"

SDL_Texture *backgroundtexture = NULL;
int vetementcount = 0;

SDL_Texture* charger_background(SDL_Renderer *renderer, const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    if (!surface) {
        printf("Erreur de chargement du fond d'écran : %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void charger_vetements_csv(const char *chemin_fichier) {
    FILE *fichier = fopen(chemin_fichier, "r");
    if (!fichier) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier : %s\n", chemin_fichier);
        return;
    }

    vetementcount = 0;
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), fichier) && vetementcount < MAX_VETEMENTS) {
        Vetement v;
        char *token = strtok(ligne, ",");
        if (!token) continue;
        strncpy(v.nom, token, sizeof(v.nom) - 1);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(v.categorie, token, sizeof(v.categorie) - 1);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(v.temperature, token, sizeof(v.temperature) - 1);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(v.motif, token, sizeof(v.motif) - 1);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(v.type, token, sizeof(v.type) - 1);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(v.couleur, token, sizeof(v.couleur) - 1);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(v.saison, token, sizeof(v.saison) - 1);

        token = strtok(NULL, "\n");
        if (!token) continue;
        strncpy(v.image_path, token, sizeof(v.image_path) - 1);

        vetements[vetementcount++] = v;
    }
    fclose(fichier);
}
void afficher_vetements(SDL_Renderer *renderer, TTF_Font *font, bool *running, AppState *currentState) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color textColor = {153, 50, 204, 255}; // Violet

    // Affichage du fond d'écran
    if (backgroundtexture) {
        SDL_RenderCopy(renderer, backgroundtexture, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(renderer, 153, 50, 204, 255);
        SDL_RenderClear(renderer);
    }

    for (int i = 0; i < vetementcount; i++) {
        SDL_Rect rect = {10, 100 + i * 180, 460, 160};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);

        // Chargement de l'image
        if (strlen(vetements[i].image_path) > 0) {
            SDL_Surface *imageSurface = IMG_Load(vetements[i].image_path);
            if (imageSurface) {
                SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
                SDL_Rect imageRect = {rect.x + 10, rect.y + 10, 80, 80}; // Ajustement
                SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
                SDL_FreeSurface(imageSurface);
                SDL_DestroyTexture(imageTexture);
            }
        }

        // Affichage du texte avec meilleur espacement
        char buffer[100];
        int textY = rect.y + 10;
        int textX = rect.x + 100;

        const char *infos[] = {
            vetements[i].nom,
            vetements[i].categorie
        };
        const char *labels[] = {
            "Nom : ",
            "Categorie : "
        };

        for (int j = 0; j < 2; j++) {
            snprintf(buffer, sizeof(buffer), "%s%s", labels[j], infos[j]);
            SDL_Surface *surface = TTF_RenderText_Solid(font, buffer, textColor);
            if (surface) {
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_Rect textRect = {textX, textY, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
                textY += 30;  // Meilleur espacement
            }
        }
    }

    // Affichage de la barre de recherche
    render_crea_bar(renderer, font, 480, white);
    SDL_RenderPresent(renderer);

}


// Dans renderer_vet()
int renderer_vet(SDL_Renderer *renderer) {
    SDL_Color white = {255, 255, 255, 255};



    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/msttcorefonts/Georgia_Bold_Italic.ttf", 20);
    

    backgroundtexture = charger_background(renderer, "background_welcome.png");

    charger_vetements_csv("vetement.csv");

    bool running = true;
    AppState currentState = STATE_VETEMENTS; // Initialisation à l'état des vêtements

    SDL_Event event;
    bool in_vet = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // Gérer l'événement de clic sur la barre de recherche
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;

                // // Si le clic est dans la zone de la barre de recherche, on change l'état pour revenir à la création
                // if (x >= 0 && x <= 480 && y >= 0 && y <= 50) { // Zone de la barre de recherche
                //     currentState = STATE_CREATION; // Revenir à la page de création
                //     in_vet = false;

                // }
                // Si le clic est dans la zone de la barre de recherche, on change l'état pour revenir à la création
                if (x >= 0 && x <= 480 && y >= 0 && y <= 50) { // Zone de la barre de recherche
                    running = false;

                }
            }
        }

        afficher_vetements(renderer, font, &running, &currentState);
    }

    if (backgroundtexture) SDL_DestroyTexture(backgroundtexture);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
   

    return 0;
}


// #include <SDL2/SDL.h>
// #include <SDL2/SDL_ttf.h>
// #include <SDL2/SDL_image.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include "vetements.h"
// #include "ajouter_vêtement.h"


// int vetementcount = 0;

// void charger_vetements_csv(const char *chemin_fichier) {
//     FILE *fichier = fopen(chemin_fichier, "r");
//     if (!fichier) {
//         fprintf(stderr, "Erreur lors de l'ouverture du fichier : %s\n", chemin_fichier);
//         return;
//     }

//     vetementcount = 0;
//     char ligne[256];

//     while (fgets(ligne, sizeof(ligne), fichier) && vetementcount < MAX_VETEMENTS) {
//         Vetement v;
//         char *token = strtok(ligne, ",");
//         if (!token) continue;
//         strncpy(v.nom, token, sizeof(v.nom) - 1);

//         token = strtok(NULL, ",");
//         if (!token) continue;
//         strncpy(v.categorie, token, sizeof(v.categorie) - 1);

//         token = strtok(NULL, ",");
//         if (!token) continue;
//         strncpy(v.temperature, token, sizeof(v.temperature) - 1);

//         token = strtok(NULL, ",");
//         if (!token) continue;
//         strncpy(v.motif, token, sizeof(v.motif) - 1);

//         token = strtok(NULL, ",");
//         if (!token) continue;
//         strncpy(v.type, token, sizeof(v.type) - 1);

//         token = strtok(NULL, ",");
//         if (!token) continue;
//         strncpy(v.couleur, token, sizeof(v.couleur) - 1);

//         token = strtok(NULL, ",");
//         if (!token) continue;
//         strncpy(v.saison, token, sizeof(v.saison) - 1);

//         token = strtok(NULL, "\n");
//         if (!token) continue;
//         strncpy(v.image_path, token, sizeof(v.image_path) - 1);

//         vetements[vetementcount++] = v;
//     }
//     fclose(fichier);
// }

// void afficher_vetements(SDL_Renderer *renderer, TTF_Font *font) {
//     SDL_Color textColor = {255, 255, 255, 255};
//     SDL_SetRenderDrawColor(renderer, 153, 50, 204, 255);
//     SDL_RenderClear(renderer);

//     if (vetementcount == 0) {
//         printf("Aucun vêtement à afficher.\n");
//     } else {
//         printf("Affichage des %d vêtements\n", vetementcount);
//     }

//     for (int i = 0; i < vetementcount; i++) {
//         SDL_Rect rect = {10, 30 + i * 180, 460, 160};
//         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//         SDL_RenderDrawRect(renderer, &rect);

//         if (strlen(vetements[i].image_path) > 0) {
//             SDL_Surface *imageSurface = IMG_Load(vetements[i].image_path);
//             if (imageSurface) {
//                 SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
//                 SDL_Rect imageRect = {rect.x + 5, rect.y + 5, 50, 50};
//                 SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
//                 SDL_FreeSurface(imageSurface);
//                 SDL_DestroyTexture(imageTexture);
//             } else {
//                 printf("Erreur de chargement de l'image : %s\n", SDL_GetError());
//             }
//         }

//         char buffer[200];
//         snprintf(buffer, sizeof(buffer), "Nom: %s", vetements[i].nom);
//         SDL_Surface *surface = TTF_RenderText_Solid(font, buffer, textColor);
//         SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
//         SDL_Rect textRect = {rect.x + 70, rect.y + 10, surface->w, surface->h};
//         SDL_RenderCopy(renderer, texture, NULL, &textRect);
//         SDL_FreeSurface(surface);
//         SDL_DestroyTexture(texture);

//         snprintf(buffer, sizeof(buffer), "Categorie: %s", vetements[i].categorie);
//         surface = TTF_RenderText_Solid(font, buffer, textColor);
//         texture = SDL_CreateTextureFromSurface(renderer, surface);
//         textRect.y += 30;
//         SDL_RenderCopy(renderer, texture, NULL, &textRect);
//         SDL_FreeSurface(surface);
//         SDL_DestroyTexture(texture);
//     }
//     SDL_RenderPresent(renderer);
// }

// int renderer_vet() {
//     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//         printf("Erreur SDL_Init : %s\n", SDL_GetError());
//         return -1;
//     }

//     if (TTF_Init() == -1) {
//         printf("Erreur TTF_Init : %s\n", TTF_GetError());
//         SDL_Quit();
//         return -1;
//     }

//     if (IMG_Init(IMG_INIT_PNG) == 0) {
//         printf("Erreur IMG_Init : %s\n", IMG_GetError());
//         TTF_Quit();
//         SDL_Quit();
//         return -1;
//     }

//     SDL_Window *window = SDL_CreateWindow("My Dressing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 800, SDL_WINDOW_SHOWN);
//     if (!window) {
//         printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
//         IMG_Quit();
//         TTF_Quit();
//         SDL_Quit();
//         return -1;
//     }

//     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     if (!renderer) {
//         printf("Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
//         SDL_DestroyWindow(window);
//         IMG_Quit();
//         TTF_Quit();
//         SDL_Quit();
//         return -1;
//     }

//     TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/msttcorefonts/Georgia_Bold_Italic.ttf", 24);
//     if (!font) {
//         printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
//         SDL_DestroyRenderer(renderer);
//         SDL_DestroyWindow(window);
//         IMG_Quit();
//         TTF_Quit();
//         SDL_Quit();
//         return -1;
//     }

//     charger_vetements_csv("vetement.csv");

//     SDL_Event event;
//     int running = 1;
//     while (running) {
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_QUIT) {
//                 running = 0;
//             }
//         }
//         afficher_vetements(renderer, font);
//     }

//     TTF_CloseFont(font);
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     IMG_Quit();
//     TTF_Quit();
//     SDL_Quit();
//     return 0;
// }


