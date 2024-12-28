#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <time.h>
#include "barre_nav.h"
#include <string.h>
#include "calendar.h"
#include "render_text.h"
#include "nav.h"  
#include "search_bar.h"

#define DAYS_IN_MONTH 31
#define MAX_TENUES_PER_DAY 10

// Tableau des jours et tenues
JourTenues calendrier[DAYS_IN_MONTH] = {0};

// Fonction pour obtenir le mois actuel
const char* get_current_month(int *year) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    *year = tm.tm_year + 1900;

    const char *months[] = {"Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin",
                            "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre"};
    return months[tm.tm_mon];
}

// Fonction pour générer une tenue aléatoire
void generer_tenue(char *tenue) {
    // Choix aléatoire des éléments de la tenue
    const char *haut = hauts[rand() % 4];
    const char *pantalon = pantalons[rand() % 4];
    const char *chaussure = chaussures[rand() % 4];

    // Calculer la longueur totale des chaînes
    size_t total_length = strlen(haut) + strlen(pantalon) + strlen(chaussure) + 2; // +2 pour les virgules et espaces

    // Vérifier si la taille totale dépasse la capacité de 'tenue'
    if (total_length < MAX_LEN) {
        snprintf(tenue, MAX_LEN, "%s, %s, %s", haut, pantalon, chaussure);
    } else {
        // Si la chaîne est trop longue, tronquer chaque élément individuellement
        snprintf(tenue, MAX_LEN, "%s, %s, %s", 
                 (strlen(haut) < MAX_LEN / 3 ? haut : "Trop long"), 
                 (strlen(pantalon) < MAX_LEN / 3 ? pantalon : "Trop long"), 
                 (strlen(chaussure) < MAX_LEN / 3 ? chaussure : "Trop long"));
    }
}


// Fonction de rendu du calendrier
void render_calendar(SDL_Renderer *renderer, TTF_Font *font, bool *running, int window_width, int window_height, AppState *currentState) {
    srand(time(NULL));  // Initialisation du générateur de nombres aléatoires

    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color purple = {128, 0, 128, 255};

    // Récupération du mois et de l'année actuels
    int year;
    const char *month_name = get_current_month(&year);

    // Taille de la grille
    const int cols = 7;  // 7 jours dans une semaine
    const int rows = 5;  // Ajustez pour le nombre de semaines
    const int cell_width = 60, cell_height = 100;

    // Calcul des dimensions de la grille
    int total_width = cols * cell_width;
    int total_height = rows * cell_height;

    // Position centrale pour le calendrier
    int grid_x = (window_width - total_width) / 2;
    int grid_y = (window_height - total_height - 60) / 2;  // Décalage pour la barre de navigation

    SDL_Event event;

    // Boucle principale de gestion des événements
    while (*running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;

                // Vérifier si un jour est cliqué
                for (int i = 0; i < DAYS_IN_MONTH; i++) {
                    int day_x = grid_x + (i % cols) * cell_width;
                    int day_y = grid_y + (i / cols) * cell_height;

                    SDL_Rect day_rect = {day_x, day_y, cell_width, cell_height};
                    if (SDL_PointInRect(&(SDL_Point){x, y}, &day_rect)) {
                        if (calendrier[i].nombre_tenues < MAX_TENUES_PER_DAY) {
                            char tenue[MAX_LEN];
                            generer_tenue(tenue);
                            strcpy(calendrier[i].tenues[calendrier[i].nombre_tenues], tenue);
                            calendrier[i].nombre_tenues++;
                        }
                    }
                }

                // Vérifier si le bouton "Création" dans la barre de navigation est cliqué
                // Utilisez les coordonnées du bouton dans la barre de navigation
                if (is_click_on_creation_button(x, y, window_width, window_height)) {
                    *currentState = STATE_CREATION;  // Passer à l'état création
                }
            }
        }

        // Nettoyage de l'écran
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Afficher le mois et l'année
        char header[50];
        snprintf(header, sizeof(header), "%s %d", month_name, year);

        int text_width, text_height;
        TTF_SizeText(font, header, &text_width, &text_height);
        int header_x = (window_width - text_width) / 2;
        int header_y = grid_y - text_height - 10; // Position juste au-dessus de la grille
        render_text(renderer, font, header, header_x, header_y, black);

        // Affichage de la grille
        for (int i = 0; i < DAYS_IN_MONTH; i++) {
            int x = grid_x + (i % cols) * cell_width;
            int y = grid_y + (i / cols) * cell_height;

            SDL_Rect cell_rect = {x, y, cell_width, cell_height};

            // Dessiner un rectangle violet pour le jour
            SDL_SetRenderDrawColor(renderer, purple.r, purple.g, purple.b, purple.a);
            SDL_RenderFillRect(renderer, &cell_rect);

            // Ajouter une bordure blanche
            SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
            SDL_RenderDrawRect(renderer, &cell_rect);

            // Afficher le numéro du jour
            char day_label[10];
            snprintf(day_label, sizeof(day_label), "%d", i + 1);
            render_text(renderer, font, day_label, x + 10, y + 10, white);

            // Afficher un résumé si des tenues sont présentes
            if (calendrier[i].nombre_tenues > 0) {
                render_text(renderer, font, "Tenues", x + 10, y + 50, white);
            }
        }
        // Dessiner la barre de recherche
        renderSearchBar(renderer, font, window_width, white);
        // Afficher la barre de navigation (incluant le bouton Création)
        render_navigation(renderer, font, window_width);

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
    }
}

// Fonction pour vérifier si le clic se trouve dans le bouton "Création" de la barre de navigation
int is_click_on_creation_button(int x, int y, int window_width, int window_height) {
    // Suppose que la barre de navigation est en bas de l'écran et le bouton "Création" est dans cette barre
    int nav_height = 60;  // Hauteur de la barre de navigation
    int button_width = 150;  // Largeur du bouton
    int button_height = 40;  // Hauteur du bouton
    int button_x = (window_width - button_width) / 2;
    int button_y = window_height - nav_height + (nav_height - button_height) / 2;

    // Vérifier si le clic est dans la zone du bouton "Création"
    SDL_Rect creation_button = {button_x, button_y, button_width, button_height};
    return SDL_PointInRect(&(SDL_Point){x, y}, &creation_button);
}
