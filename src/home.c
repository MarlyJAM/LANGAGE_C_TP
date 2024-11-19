#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

// Dimensions de la fenêtre
const int WINDOW_WIDTH = 480;
const int WINDOW_HEIGHT = 800;

// Fonction pour rendre du texte à l'écran
void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    // Créer une surface de texte
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    // Créer une texture à partir de la surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    // Définir la position et la taille du texte
    SDL_Rect dest = { x, y, surface->w, surface->h };
    // Copier la texture à l'écran
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    // Libérer la surface et la texture
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Fonction pour dessiner un rectangle avec bordure
void draw_rect_with_border(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color fill_color, SDL_Color border_color, int border_width) {
    // Dessiner la bordure
    SDL_SetRenderDrawColor(renderer, border_color.r, border_color.g, border_color.b, border_color.a);
    for (int i = 0; i < border_width; ++i) {
        SDL_Rect border_rect = { rect.x - i, rect.y - i, rect.w + 2*i, rect.h + 2*i };
        SDL_RenderDrawRect(renderer, &border_rect);
    }
    // Dessiner l'intérieur du rectangle
    SDL_SetRenderDrawColor(renderer, fill_color.r, fill_color.g, fill_color.b, fill_color.a);
    SDL_RenderFillRect(renderer, &rect);
}

int main(int argc, char* argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    // Initialisation de SDL_ttf
    if (TTF_Init() != 0) {
        SDL_Quit();
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("My Dressing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

    // Chargement de la police de caractères
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf", 24);
    if (font == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Définition des couleurs
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color purple = { 128, 0, 128, 255 };
    SDL_Color border_color = { 255, 215, 0, 255 }; // Couleur or pour la bordure

    bool running = true;
    SDL_Event event;

    // Boucle principale
    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Effacer l'écran avec une couleur de fond violette
        SDL_SetRenderDrawColor(renderer, 75, 0, 130, 255);  // Fond violet
        SDL_RenderClear(renderer);

        // Dessiner le bouton "COMMENCER" avec une bordure
        SDL_Rect button_rect = { 140, 50, 200, 60 };
        draw_rect_with_border(renderer, button_rect, white, border_color, 3);
        render_text(renderer, font, "COMMENCER", 150, 60, purple);

        // Dessiner le texte "MY DRESSING" avec une bordure
        SDL_Rect text_rect = { 90, 300, 300, 100 };
        draw_rect_with_border(renderer, text_rect, purple, border_color, 3);
        render_text(renderer, font, "MY DRESSING", 140, 320, white);

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }

    // Libérer les ressources
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
