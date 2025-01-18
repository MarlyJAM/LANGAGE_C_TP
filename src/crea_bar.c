#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "crea_bar.h"

// Fonction pour afficher la barre de recherche centrée en haut de la fenêtre
void render_crea_bar(SDL_Renderer *renderer, TTF_Font *font, int window_width, SDL_Color color) {
    // Définition de la barre de recherche
    SDL_Rect creaBar = {0, 0, window_width, 50};

    // Couleur violette pour la barre de recherche
    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
    SDL_RenderFillRect(renderer, &creaBar);

    // Affichage du texte centré dans la barre de recherche
    SDL_Surface *surface = TTF_RenderText_Blended(font, "MY DRESSING", color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {
        creaBar.x + ( creaBar.w - surface->w) / 2, // Centrer le texte horizontalement
        creaBar.y + ( creaBar.h - surface->h) / 2, // Centrer le texte verticalement
        surface->w,
        surface->h
    };


    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}