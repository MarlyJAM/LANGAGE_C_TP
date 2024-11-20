#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "calendar.h"
#include "render_text.h"

// Structure pour gérer les tenues (déjà défini dans le code fourni)
#define DAYS_IN_MONTH 30
#define MAX_LEN 100
#define MAX_TENUES_PER_DAY 10

// Tableau des jours et tenues
JourTenues calendrier[DAYS_IN_MONTH] = {0};

// Fonction pour générer une tenue aléatoire
void generer_tenue(char *tenue) {
    const char *hauts[] = {"T-shirt noir", "T-shirt blanc", "Pull", "Chemise"};
    const char *pantalons[] = {"Jean", "Pantalon noir", "Jupe", "Short"};
    const char *chaussures[] = {"Baskets blanches", "Chaussures noires", "Sandales", "Bottes"};

    snprintf(tenue, MAX_LEN, "%s, %s, %s",
             hauts[rand() % (sizeof(hauts) / sizeof(hauts[0]))],
             pantalons[rand() % (sizeof(pantalons) / sizeof(pantalons[0]))],
             chaussures[rand() % (sizeof(chaussures) / sizeof(chaussures[0]))]);
}

// Fonction pour afficher la page calendrier
void render_calendar(SDL_Renderer *renderer, TTF_Font *font, bool *running) {
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color purple = {128, 0, 128, 255};
    
    // Taille de la grille
    const int cols = 5, rows = 6;
    const int cell_width = 80, cell_height = 80;
    const int grid_x = 50, grid_y = 50;

    // Variable pour capturer les événements
    SDL_Event event;

    // Boucle d'affichage de la page calendrier
    while (*running) {
        // Gestion des événements
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
                        // Ajouter une tenue pour le jour cliqué
                        if (calendrier[i].nombre_tenues < MAX_TENUES_PER_DAY) {
                            char tenue[MAX_LEN];
                            generer_tenue(tenue);
                            strcpy(calendrier[i].tenues[calendrier[i].nombre_tenues], tenue);
                            calendrier[i].nombre_tenues++;
                        }
                    }
                }
            }
        }

        // Nettoyage de l'écran
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Affichage de la grille des jours
        for (int i = 0; i < DAYS_IN_MONTH; i++) {
            int x = grid_x + (i % cols) * cell_width;
            int y = grid_y + (i / cols) * cell_height;

            // Dessiner un carré pour le jour
            SDL_Rect cell_rect = {x, y, cell_width, cell_height};
            SDL_SetRenderDrawColor(renderer, purple.r, purple.g, purple.b, purple.a);
            SDL_RenderFillRect(renderer, &cell_rect);

            // Afficher le numéro du jour
            char day_label[10];
            snprintf(day_label, sizeof(day_label), "%d", i + 1);
            render_text(renderer, font, day_label, x + 10, y + 10, white);

            // Afficher un résumé des tenues
            if (calendrier[i].nombre_tenues > 0) {
                render_text(renderer, font, "Tenues", x + 10, y + 40, white);
            }
        }

        // Afficher un bouton pour quitter
        SDL_Rect quit_button = {400, 500, 80, 40};
        SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
        SDL_RenderFillRect(renderer, &quit_button);
        render_text(renderer, font, "Quitter", 410, 510, white);

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }
}
