#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "nav.h"
#include "création.h"
#include "calendar.h"
#include "barre_nav.h"
#include "render_text.h"  



// Couleurs utilisées pour les boutons
SDL_Color white = {255, 255, 255, 255};
SDL_Color purple = {128, 0, 128, 255};
SDL_Color border_color = {255, 105, 180, 255};

// Fonction pour afficher la barre de navigation
void render_navigation(SDL_Renderer *renderer, TTF_Font *font, int window_width) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color purple = {128, 0, 128, 255}; // Couleur de fond des boutons (violet)
    SDL_Color border_color = {255, 105, 180, 255}; // Couleur des bordures

    int nav_width = 420; // Largeur de la zone de navigation
    int nav_x = (window_width - nav_width) / 2; // Centrer la zone de navigation
    int button_y = 700; // Position verticale des boutons
    int button_margin = 20;

    // Définir les zones des boutons
    SDL_Rect button1_rect = {nav_x + button_margin, button_y, NAV_BUTTON_WIDTH, NAV_BUTTON_HEIGHT};  // Calendrier
    SDL_Rect button2_rect = {nav_x + nav_width / 2 - NAV_BUTTON_WIDTH / 2, button_y, NAV_BUTTON_WIDTH, NAV_BUTTON_HEIGHT};  // Tenues
    SDL_Rect button3_rect = {nav_x + nav_width - NAV_BUTTON_WIDTH - button_margin, button_y, NAV_BUTTON_WIDTH, NAV_BUTTON_HEIGHT};  // Création

    // Dessiner les boutons avec bordures
    SDL_SetRenderDrawColor(renderer, purple.r, purple.g, purple.b, purple.a);
    SDL_RenderFillRect(renderer, &button1_rect); // Remplir le bouton Calendrier
    SDL_SetRenderDrawColor(renderer, border_color.r, border_color.g, border_color.b, border_color.a);
    SDL_RenderDrawRect(renderer, &button1_rect); // Dessiner la bordure Calendrier

    SDL_RenderFillRect(renderer, &button2_rect); // Remplir le bouton Tenues
    SDL_RenderDrawRect(renderer, &button2_rect); // Dessiner la bordure Tenues

    SDL_RenderFillRect(renderer, &button3_rect); // Remplir le bouton Création
    SDL_RenderDrawRect(renderer, &button3_rect); // Dessiner la bordure Création

    // Dessiner le texte sur chaque bouton
    int text_width, text_height;

    // Texte du bouton Calendrier
    TTF_SizeText(font, "Agenda", &text_width, &text_height);
    int text_x1 = button1_rect.x + (button1_rect.w - text_width) / 2;
    int text_y1 = button1_rect.y + (button1_rect.h - text_height) / 2;
    render_text(renderer, font, "Agenda", text_x1, text_y1, white);

    // Texte du bouton Tenues
    TTF_SizeText(font, "Tenues", &text_width, &text_height);
    int text_x2 = button2_rect.x + (button2_rect.w - text_width) / 2;
    int text_y2 = button2_rect.y + (button2_rect.h - text_height) / 2;
    render_text(renderer, font, "Tenues", text_x2, text_y2, white);

    // Texte du bouton Création
    TTF_SizeText(font, "Creation", &text_width, &text_height);
    int text_x3 = button3_rect.x + (button3_rect.w - text_width) / 2;
    int text_y3 = button3_rect.y + (button3_rect.h - text_height) / 2;
    render_text(renderer, font, "Creation", text_x3, text_y3, white);
}


// Fonction pour gérer les clics sur la barre de navigation
void handle_navigation_click(int x, int y, int window_width, AppState *currentState) {
    // Vérifier si le clic est dans l'une des zones de bouton de la barre de navigation
    int button_width = NAV_BUTTON_WIDTH;
    int button_height = NAV_BUTTON_HEIGHT;
    
    int nav_x = (window_width - 420) / 2;  // Centrer la barre de navigation
    int button_y = 700;  // Position des boutons de navigation

    // Zone pour chaque bouton
    SDL_Rect button1 = {nav_x, button_y, button_width, button_height}; // Calendrier
    SDL_Rect button2 = {nav_x + 140, button_y, button_width, button_height}; // Tenues
    SDL_Rect button3 = {nav_x + 280, button_y, button_width, button_height}; // Création

    if (is_point_in_rect(x, y, &button1)) {
        *currentState = STATE_CALENDAR;
        printf("Bouton cliqué");
    } else if (is_point_in_rect(x, y, &button2)) {
        *currentState = STATE_TENUES;
         printf("Bouton cliqué");
    } else if (is_point_in_rect(x, y, &button3)) {
        *currentState = STATE_CREATION;
        printf("Bouton cliqué");
    }
}
// Vérifie si un clic est dans les limites du bouton
bool is_point_in_rect(int x, int y, SDL_Rect *rect) {
    return x >= rect->x && x <= rect->x + rect->w && y >= rect->y && y <= rect->y + rect->h;
}
