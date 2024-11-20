#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


// Fonction main pour démarrer l'application
int main(int argc, char *argv[]) {
    // Initialiser SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Ouvrir la fenêtre secondaire
    ouvrir_fenetre_secondaire();

    return 0;
}
