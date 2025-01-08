#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "barre_nav.h"
#include "search_bar.h"
#include "welcome.h"

// Fonction pour afficher la barre de recherche centrée en haut de la fenêtre
void renderSearchBar(SDL_Renderer *renderer, TTF_Font *font, int window_width, SDL_Color color, AppState *currentState) {
    // Définition de la barre de recherche
    SDL_Rect searchBar = {0, 0, window_width, 50};

    // Couleur violette pour la barre de recherche
    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
    SDL_RenderFillRect(renderer, &searchBar);

    // Affichage du texte centré dans la barre de recherche
    SDL_Surface *surface = TTF_RenderText_Blended(font, "MY DRESSING", color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {
        searchBar.x + (searchBar.w - surface->w) / 2, // Centrer le texte horizontalement
        searchBar.y + (searchBar.h - surface->h) / 2, // Centrer le texte verticalement
        surface->w,
        surface->h
    };

    // // Gestion des événements pour détecter un clic dans la barre de recherche
    // SDL_Event event;
    // while (SDL_PollEvent(&event)) {
    //     if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
    //         int x = event.button.x;
    //         int y = event.button.y;

    //         // Vérifiez si le clic est dans la zone de la barre de recherche
    //         if (x >= searchBar.x && x <= searchBar.x + searchBar.w &&
    //             y >= searchBar.y && y <= searchBar.y + searchBar.h) {
    //             // Redirection vers l'écran d'accueil (STATE_WELCOME)
    //             bool* running;
    //             printf("Bouton cliqué");
    //             render_welcome(renderer,font,running,window_width,800,currentState);
               
    //         }
    //     }
    // }

    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

/* Fonction pour gérer les clics sur la barre de recherche
bool handleSearchBarClick(SDL_Rect *searchBar, int mouse_x, int mouse_y) {
    // Vérifier si le clic est dans la zone de la barre de recherche
    if (mouse_x >= searchBar->x && mouse_x <= searchBar->x + searchBar->w &&
        mouse_y >= searchBar->y && mouse_y <= searchBar->y + searchBar->h) {
        printf("Barre de recherche cliquée ! Redirection en cours...\n");
        render_welcome();
        return true;
    }
    return false;
}*/
