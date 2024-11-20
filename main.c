#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "home.h"
#include "welcome.h"

// États de l'application
typedef enum {
    STATE_HOME,
    STATE_WELCOME
} AppState;

int main(int argc, char* argv[]) {
    // Initialisation de SDL et SDL_ttf
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        SDL_Quit();
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("My Dressing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 800, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Chargement de la police
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf", 24);
    
    if (font == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Gestion des états
    AppState currentState = STATE_HOME;
    bool running = true;

    // Boucle principale
    while (running) {
        if (currentState == STATE_HOME) {
            bool change_to_welcome = false;
            render_home(renderer, font, &change_to_welcome, &running);
            if (change_to_welcome) {
                currentState = STATE_WELCOME;
            }
        } else if (currentState == STATE_WELCOME) {
            render_welcome(renderer, font, &running);
        }
        
    }

    // Libérer les ressources
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
