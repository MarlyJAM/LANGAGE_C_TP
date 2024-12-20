#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "calendar.h"
#include "render_text.h"
#include "nav.h"  // Inclure le fichier de navigation pour la barre

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
             pantalons[rand() % (sizeof(pantalons) / sizeof(hauts[0]))],
             chaussures[rand() % (sizeof(chaussures) / sizeof(chaussures[0]))]);
}

// Fonction pour afficher la page calendrier
void render_calendar(SDL_Renderer *renderer, TTF_Font *font, bool *running) {
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color purple = {128, 0, 128, 255};

    // Affichage du mois et de l'année
    const char *month_name = "Décembre";  // Exemple de mois (à modifier dynamiquement selon le mois actuel)
    render_text(renderer, font, month_name, 150, 10, white);

    // Taille de la grille
    const int cols = 7;  // 7 jours dans une semaine
    const int rows = 5;  // 5 lignes de jours (en fonction du mois)
    const int cell_width = 60, cell_height = 60;
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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Fond blanc
        SDL_RenderClear(renderer);

        // Affichage de la grille des jours
        for (int i = 0; i < DAYS_IN_MONTH; i++) {
            int x = grid_x + (i % cols) * cell_width;
            int y = grid_y + (i / cols) * cell_height;

            // Dessiner un carré pour le jour avec un contour violet
            SDL_Rect cell_rect = {x, y, cell_width, cell_height};
            SDL_SetRenderDrawColor(renderer, purple.r, purple.g, purple.b, purple.a);
            SDL_RenderFillRect(renderer, &cell_rect);

            // Ajouter un contour arrondi (utiliser un ellipse ou cercle pour un vrai contour arrondi)
            SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
            SDL_RenderDrawRect(renderer, &cell_rect);  // Rectangle avec bord

            // Afficher le numéro du jour
            char day_label[10];
            snprintf(day_label, sizeof(day_label), "%d", i + 1);
            render_text(renderer, font, day_label, x + 10, y + 10, white);

            // Afficher un résumé des tenues
            if (calendrier[i].nombre_tenues > 0) {
                render_text(renderer, font, "Tenues", x + 10, y + 40, white);
            }
        }

        // Dessiner la barre de navigation en bas de l'écran
        render_navigation(renderer, font, 480);  // Appel de la fonction de la barre de navigation avec une largeur de fenêtre de 480 px

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }
}
