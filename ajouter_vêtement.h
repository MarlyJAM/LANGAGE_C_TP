#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


// Structure pour gérer les champs d'entrée
typedef struct {
    SDL_Rect rect;      // Rectangle du champ
    SDL_Color color;    // Couleur actuelle du champ
    const char *title;  // Titre associé au champ
    bool active;        // Indique si le champ est sélectionné
    char input[256];    // Texte saisi dans le champ
} InputField;

// Structure pour stocker les informations du vêtement
typedef struct {
    char nom[256];      // Nom du vêtement
    char categorie[256];
    char temperature[256];
    char motif[256];
    char type[256];
    char couleur[256];
    char saison[256];
} Vetement;


// Fonction pour dessiner un champ d'entrée avec son titre et son contenu
void render_field(SDL_Renderer *renderer, TTF_Font *font, InputField *field);

// Fonction pour dessiner un bouton
void render_button(SDL_Renderer *renderer, TTF_Font *font, const char *label, SDL_Rect *buttonRect) ;

// Fonction principale pour afficher et gérer la fenêtre secondaire
void ajouter_vetement(); 