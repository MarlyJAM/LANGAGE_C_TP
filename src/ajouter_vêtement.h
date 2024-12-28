#ifndef AJOUTER_VETEMENT_H
#define AJOUTER_VETEMENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define FIELD_WIDTH 320
#define FIELD_HEIGHT 40
#define FIELD_MARGIN 35
#define BUTTON_WTH 160
#define BUTTON_HEIGHT 50
#define WINDOW_WTH 480
#define WINDOW_HEIGHT 800

// Structure pour un champ de saisie
typedef struct {
    SDL_Rect rect;         // Position et dimensions du champ
    SDL_Color color;       // Couleur actuelle du champ
    const char *title;     // Titre du champ
    bool active;           // Indique si le champ est actif
    char input[256];       // Texte saisi dans le champ
} InputField;

// Structure représentant un vêtement
typedef struct {
    char nom[256];
    char categorie[256];
    char temperature[256];
    char motif[256];
    char type[256];
    char couleur[256];
    char saison[256];
} Vetement;

void render_field(SDL_Renderer *renderer, TTF_Font *font, InputField *field);
// Fonction pour afficher un champ d'importation d'image
void render_image_field(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect *imageRect, SDL_Texture *imageTexture);
void render_button(SDL_Renderer *renderer, TTF_Font *font, const char *label, SDL_Rect *buttonRect);

// Fonction pour importer une image et en retourner une texture
SDL_Texture *import_image(SDL_Renderer *renderer, const char *filePath);

// Fonction principale pour ajouter un vêtement
void ajouter_vetement();

#endif // AJOUTER_VETEMENT_H

