#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define WINDOW_WIDTH 1280  // Largeur de la fenêtre
#define WINDOW_HEIGHT 720  // Hauteur de la fenêtre
#define FIELD_WIDTH 600    // Largeur des champs
#define FIELD_HEIGHT 50    // Hauteur des champs
#define FIELD_MARGIN 20    // Espace entre les champs
#define BUTTON_WIDTH 200   // Largeur du bouton
#define BUTTON_HEIGHT 50   // Hauteur du bouton


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
void render_field(SDL_Renderer *renderer, TTF_Font *font, InputField *field) {
    // Dessiner le champ
    SDL_SetRenderDrawColor(renderer, field->color.r, field->color.g, field->color.b, 255);
    SDL_RenderFillRect(renderer, &field->rect);

    // Dessiner le titre du champ
    SDL_Color textColor = {0, 0, 0, 255}; // Noir
    SDL_Surface *titleSurface = TTF_RenderText_Solid(font, field->title, textColor);
    SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);

    int titleWidth = 0, titleHeight = 0;
    TTF_SizeText(font, field->title, &titleWidth, &titleHeight);

    SDL_Rect titleRect = {
        field->rect.x + (field->rect.w - titleWidth) / 2,
        field->rect.y - titleHeight - 10,
        titleWidth,
        titleHeight
    };
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);

    // Dessiner le contenu du champ (texte saisi)
    if (field->input[0] != '\0') { // Si le champ contient du texte
        SDL_Surface *inputSurface = TTF_RenderText_Solid(font, field->input, textColor);
        SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);

        SDL_Rect inputRect = {
            field->rect.x + 10,  // Marges intérieures pour le texte
            field->rect.y + (field->rect.h - titleHeight) / 2,
            inputSurface->w,
            inputSurface->h
        };
        SDL_RenderCopy(renderer, inputTexture, NULL, &inputRect);

        SDL_FreeSurface(inputSurface);
        SDL_DestroyTexture(inputTexture);
    }
}

// Fonction pour dessiner un bouton
void render_button(SDL_Renderer *renderer, TTF_Font *font, const char *label, SDL_Rect *buttonRect) {
    // Dessiner le bouton
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Vert
    SDL_RenderFillRect(renderer, buttonRect);

    // Dessiner le texte sur le bouton
    SDL_Color textColor = {0, 0, 0, 255}; // Noir
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, label, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth = 0, textHeight = 0;
    TTF_SizeText(font, label, &textWidth, &textHeight);

    SDL_Rect textRect = {
        buttonRect->x + (buttonRect->w - textWidth) / 2,
        buttonRect->y + (buttonRect->h - textHeight) / 2,
        textWidth,
        textHeight
    };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// Fonction principale pour afficher et gérer la fenêtre secondaire
void ouvrir_fenetre_secondaire() {
    SDL_Window *fenetre = SDL_CreateWindow(
        "Ajouter un vêtement",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!fenetre) {
        fprintf(stderr, "Erreur création fenêtre secondaire : %s\n", SDL_GetError());
        return;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Erreur création renderer secondaire : %s\n", SDL_GetError());
        SDL_DestroyWindow(fenetre);
        return;
    }

    if (TTF_Init() != 0) {
        fprintf(stderr, "Erreur initialisation TTF : %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        return;
    }

    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf", 24);
    if (!font) {
        fprintf(stderr, "Erreur chargement police : %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        return;
    }

    // Définition des champs d'entrée
    InputField fields[7];  // Mise à jour pour inclure un champ supplémentaire
    const char *titles[] = {"Nom", "Categorie", "Température", "Motif", "Type", "Couleur", "Saison"};
    SDL_Color defaultColor = {255, 0, 0, 255}; // Rouge par défaut
    SDL_Color activeColor = {0, 255, 0, 255};  // Vert si actif

    for (int i = 0; i < 7; i++) {  // Mise à jour de la boucle pour 7 champs
        fields[i].rect.x = (WINDOW_WIDTH - FIELD_WIDTH) / 2;
        fields[i].rect.y = 100 + i * (FIELD_HEIGHT + FIELD_MARGIN);
        fields[i].rect.w = FIELD_WIDTH;
        fields[i].rect.h = FIELD_HEIGHT;
        fields[i].color = defaultColor;
        fields[i].title = titles[i];
        fields[i].active = false;
        fields[i].input[0] = '\0';  // Champ vide au départ
    }

    // Définir le bouton "Valider"
    SDL_Rect buttonRect = { (WINDOW_WIDTH - BUTTON_WIDTH) / 2, WINDOW_HEIGHT - BUTTON_HEIGHT - 50, BUTTON_WIDTH, BUTTON_HEIGHT };

    // Initialisation du clavier virtuel
    SDL_StartTextInput(); // Active la saisie de texte

    // Structure pour stocker le vêtement
    Vetement newVetement;

    // Boucle principale
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // Gestion des clics ou tapotements
            if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_FINGERDOWN) {
                int x = event.type == SDL_MOUSEBUTTONDOWN ? event.button.x : (int)(event.tfinger.x * WINDOW_WIDTH);
                int y = event.type == SDL_MOUSEBUTTONDOWN ? event.button.y : (int)(event.tfinger.y * WINDOW_HEIGHT);

                // Vérification du clic sur les champs
                for (int i = 0; i < 7; i++) {  // Mise à jour pour 7 champs
                    if (x >= fields[i].rect.x && x <= fields[i].rect.x + fields[i].rect.w &&
                        y >= fields[i].rect.y && y <= fields[i].rect.y + fields[i].rect.h) {
                        fields[i].active = true;
                        fields[i].color = activeColor;
                    } else {
                        fields[i].active = false;
                        fields[i].color = defaultColor;
                    }
                }

                // Vérification du clic sur le bouton "Valider"
                if (x >= buttonRect.x && x <= buttonRect.x + buttonRect.w &&
                    y >= buttonRect.y && y <= buttonRect.y + buttonRect.h) {
                    // Sauvegarder les valeurs dans la structure Vetement
                    strcpy(newVetement.nom, fields[0].input);
                    strcpy(newVetement.categorie, fields[1].input);
                    strcpy(newVetement.temperature, fields[2].input);
                    strcpy(newVetement.motif, fields[3].input);
                    strcpy(newVetement.type, fields[4].input);
                    strcpy(newVetement.couleur, fields[5].input);
                    strcpy(newVetement.saison, fields[6].input);

                    // Afficher les informations sauvegardées (pour tester)
                    printf("Nom: %s\n", newVetement.nom);
                    printf("Categorie: %s\n", newVetement.categorie);
                    printf("Température: %s\n", newVetement.temperature);
                    printf("Motif: %s\n", newVetement.motif);
                    printf("Type: %s\n", newVetement.type);
                    printf("Couleur: %s\n", newVetement.couleur);
                    printf("Saison: %s\n", newVetement.saison);
                }
            }

            // Gestion de la saisie de texte dans les champs actifs
            if (event.type == SDL_TEXTINPUT) {
                for (int i = 0; i < 7; i++) {
                    if (fields[i].active) {
                        strncat(fields[i].input, event.text.text, sizeof(fields[i].input) - strlen(fields[i].input) - 1);
                    }
                }
            }

            // Gestion de la suppression de texte
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE) {
                for (int i = 0; i < 7; i++) {
                    if (fields[i].active && strlen(fields[i].input) > 0) {
                        fields[i].input[strlen(fields[i].input) - 1] = '\0';
                    }
                }
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Blanc
        SDL_RenderClear(renderer);

        // Dessiner les champs et le bouton
        for (int i = 0; i < 7; i++) {
            render_field(renderer, font, &fields[i]);
        }
        render_button(renderer, font, "Valider", &buttonRect);

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
    }

    // Libérer les ressources
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    TTF_Quit();
    SDL_Quit();
}