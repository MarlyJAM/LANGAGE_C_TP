#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "barre_nav.h"
#include "nav.h"
#include "home.h"
#include "welcome.h"
#include "calendar.h"
#include "création.h"

// Déclaration de l'état actuel de l'application
AppState currentState = STATE_HOME;

// Dimensions de la fenêtre
int window_width = 480;
int window_height = 800;

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
    SDL_Window* window = SDL_CreateWindow("My Dressing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
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
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/msttcorefonts/Georgia_Bold_Italic.ttf", 24);
    if (font == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    bool running = true;
    bool change_to_welcome = false;

        // Boucle principale
    while (running) {
        if (currentState == STATE_HOME) {
            // Gérer l'affichage de la page d'accueil (home)
            render_home(renderer, font, &change_to_welcome, &running);  // Passer change_to_welcome ici

            // Si l'utilisateur clique sur "Commencer", on passe à la vue suivante
            if (change_to_welcome) {
                currentState = STATE_WELCOME;
                change_to_welcome = false;  // Réinitialiser l'indicateur
            }
        } else if (currentState == STATE_WELCOME) {
            // Gérer l'affichage de la page de bienvenue (welcome)
            render_welcome(renderer, font, &running, window_width, window_height, &currentState);  // Ajouter currentState ici
        } else if (currentState == STATE_CALENDAR || currentState == STATE_CREATION || currentState == STATE_TENUES) {
            // Afficher la barre de navigation uniquement dans ces états
            render_navigation(renderer, font, window_width);  // Affichage de la barre de navigation

            // Afficher l'écran correspondant à l'état actuel
            if (currentState == STATE_CALENDAR) {
                render_calendar(renderer, font, &running, window_width, window_height, &currentState);
            } else if (currentState == STATE_CREATION) {
                render_creation(renderer, font, &running, window_width);
            } /*else if (currentState == STATE_TENUES) {
                render_tenues(renderer, font, &running); // Assurez-vous d'implémenter render_tenues si nécessaire
            }*/
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
