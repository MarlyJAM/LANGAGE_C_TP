#ifndef VETEMENT_MANAGER_H
#define VETEMENT_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tinyfiledialogs.h" 
#include "accessoire.h"

#define MAX_PATH_LENGTH 200
#define W_WIDTH 480
#define W_HEIGHT 800
#define F_WIDTH 300
#define F_HEIGHT 40
#define F_MARGIN 50 
#define B_WIDTH 200
#define B_HEIGHT 40


typedef struct {
    SDL_Rect rect;
    char title[50];
    char input[100];
    SDL_Color color;
    bool active;
} InputField;
// Structure représentant un vêtement
typedef struct Vetement {
    char nom[50];
    char categorie[50];
    char temperature[20];
    char motif[50];
    char type[50];
    char couleur[30];
    char saison[20];
    char image[MAX_PATH_LENGTH];
    char image_path[MAX_PATH_LENGTH]; // Chemin complet de l'image
} Vetement;

// Variables globales pour gérer les vêtements
extern Vetement vetements[MAX_VETEMENTS];


// Fonction pour copier une image dans un dossier cible
bool copy_image_to_folder(const char *src, const char *dest_folder, char *dest_path) ;

// Fonction pour ajouter un vêtement
void ajouter_vetement(SDL_Renderer *renderer, TTF_Font *font);
void render_button(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect *buttonRect, const char *text);
void render_field(SDL_Renderer *renderer, TTF_Font *font, InputField *field);
void sauvegarder_vetement_csv(const char *chemin_fichier, Vetement *vetement);
// Prototype de la fonction charger_image
SDL_Texture* chargerimage(const char *fichier, SDL_Renderer *renderer);
SDL_Texture* import_image(SDL_Renderer *renderer, const char *file_path);


// Fonction pour afficher les vêtements dans la console
//void afficher_vetements();

#endif // VETEMENT_MANAGER_H

