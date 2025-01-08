#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "search_bar.h"
#include "ajouter_vêtement.h"
#include "tinyfiledialogs.h"
#include <stdlib.h>

// Fonction pour afficher un champ d'entrée avec son titre et son contenu
void render_field(SDL_Renderer *renderer, TTF_Font *font, InputField *field) {
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

    // Dessiner le champ
    SDL_SetRenderDrawColor(renderer, field->color.r, field->color.g, field->color.b, 255);
    SDL_RenderFillRect(renderer, &field->rect);

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

// Fonction pour afficher un bouton
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

// Fonction pour importer une image
SDL_Texture* import_image(SDL_Renderer *renderer, const char *file_path) {
    SDL_Surface *imageSurface = IMG_Load(file_path);
    if (!imageSurface) {
        fprintf(stderr, "Erreur chargement image : %s\n", IMG_GetError());
        return NULL;
    }

    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);

    return imageTexture;
}

// Fonction pour dessiner un champ spécifique pour l'image
void render_image_field(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect *rect, SDL_Texture *imageTexture) {
    // Dessiner le cadre en violet foncé
    SDL_SetRenderDrawColor(renderer, 75, 0, 130, 255); // Violet foncé
    SDL_RenderFillRect(renderer, rect);

    if (imageTexture) {
        // Si une image est disponible, afficher une miniature
        SDL_Rect imageRect = {
            rect->x + 10, rect->y + 5,
            rect->w - 20, rect->h - 10
        };
        SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
    } else {
        // Si aucune image, afficher le texte "Importer une image" en blanc
        SDL_Color textColor = {255, 255, 255, 255}; // Blanc
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Importer une image", textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        int textWidth, textHeight;
        TTF_SizeText(font, "Importer une image", &textWidth, &textHeight);

        SDL_Rect textRect = {
            rect->x + (rect->w - textWidth) / 2,
            rect->y + (rect->h - textHeight) / 2,
            textWidth,
            textHeight
        };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

// Fonction pour écrire les données dans un fichier CSV
void enregistrer_vetement_csv(const char *fichier, Vetement *vetement, const char *image_path) {
    FILE *file = fopen(fichier, "a"); // Mode "append" pour ajouter sans écraser les anciennes données
    if (!file) {
        fprintf(stderr, "Erreur d'ouverture du fichier CSV : %s\n", fichier);
        return;
    }
    // Écrire les données dans le fichier (nom de l'image)
    fprintf(file, "%s,%s,%s,%s,%s,%s,%s,%s\n",
            vetement->nom,
            vetement->categorie,
            vetement->temperature,
            vetement->motif,
            vetement->type,
            vetement->couleur,
            vetement->saison,
            image_path ? image_path : "Aucune image");

    fclose(file);
}



void ajouter_vetement() {
    SDL_Color white = {255,255,255,255};
    SDL_Window *fenetre = SDL_CreateWindow(
        "Ajouter un vêtement",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WTH,
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

    if (TTF_Init() != 0 || IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        fprintf(stderr, "Erreur initialisation des bibliothèques : %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        return;
    }

    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf", 18);
    if (!font) {
        fprintf(stderr, "Erreur chargement police : %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        IMG_Quit();
        return;
    }

    InputField fields[7];
    const char *titles[] = {"Nom", "Categorie", "Temperature", "Motif", "Type", "Couleur", "Saison"};
    SDL_Color defaultColor = {200, 200, 0, 255};
    SDL_Color activeColor = {200, 200, 0, 100};

    for (int i = 0; i < 7; i++) {
        fields[i].rect.x = (WINDOW_WTH - FIELD_WIDTH) / 2;
        fields[i].rect.y = 100 + i * (FIELD_HEIGHT + FIELD_MARGIN);
        fields[i].rect.w = FIELD_WIDTH;
        fields[i].rect.h = FIELD_HEIGHT;
        fields[i].color = defaultColor;
        fields[i].title = titles[i];
        fields[i].active = false;
        fields[i].input[0] = '\0';
    }
    // Définition du champ "Importer une image"
    SDL_Rect imageRect = {
        (WINDOW_WTH - FIELD_WIDTH) / 2,
        100 + 7 * (FIELD_HEIGHT + FIELD_MARGIN),
        FIELD_WIDTH,
        FIELD_HEIGHT
    };

    // Définition du bouton "Valider" avec un espace ajouté
    SDL_Rect buttonRect = {
        (WINDOW_WTH - BUTTON_WTH ) / 2,
        100 + 8 * (FIELD_HEIGHT + FIELD_MARGIN) + 20, // 20px d'espace supplémentaire
        BUTTON_WTH ,
        BUTTON_HEIGHT
    };

    SDL_Texture *imageTexture = NULL;
    
    SDL_StartTextInput();

    Vetement newVetement;
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_FINGERDOWN) {
                int x = event.type == SDL_MOUSEBUTTONDOWN ? event.button.x : (int)(event.tfinger.x * WINDOW_WTH);
                int y = event.type == SDL_MOUSEBUTTONDOWN ? event.button.y : (int)(event.tfinger.y * WINDOW_HEIGHT);

                for (int i = 0; i < 7; i++) {
                    if (x >= fields[i].rect.x && x <= fields[i].rect.x + fields[i].rect.w &&
                        y >= fields[i].rect.y && y <= fields[i].rect.y + fields[i].rect.h) {
                        fields[i].active = true;
                        fields[i].color = activeColor;
                    } else {
                        fields[i].active = false;
                        fields[i].color = defaultColor;
                    }
                }

                if (x >= imageRect.x && x <= imageRect.x + imageRect.w &&
                    y >= imageRect.y && y <= imageRect.y + imageRect.h) {
                    // Ouvrir une boîte de dialogue pour sélectionner une image
                    const char *filePath = tinyfd_openFileDialog(
                        "Choisissez une image", "", 0, NULL, NULL, 0
                    );

                    if (filePath) {
                        SDL_Texture *newTexture = import_image(renderer, filePath);
                        if (newTexture) {
                            SDL_DestroyTexture(imageTexture);
                            imageTexture = newTexture;
                        } else {
                            fprintf(stderr, "Erreur lors de l'importation de l'image : %s\n", IMG_GetError());
                        }
                    }
                }

                if (x >= buttonRect.x && x <= buttonRect.x + buttonRect.w &&
                y >= buttonRect.y && y <= buttonRect.y + buttonRect.h) {
                    // Récupérer les données des champs
                    strcpy(newVetement.nom, fields[0].input);
                    strcpy(newVetement.categorie, fields[1].input);
                    strcpy(newVetement.temperature, fields[2].input);
                    strcpy(newVetement.motif, fields[3].input);
                    strcpy(newVetement.type, fields[4].input);
                    strcpy(newVetement.couleur, fields[5].input);
                    strcpy(newVetement.saison, fields[6].input);

                    // Ajouter le chemin de l'image si elle existe
                    if (imageTexture) {
                        snprintf(newVetement.image, sizeof(newVetement.image), "%s", tinyfd_getFilePath());
                        } else {
                            strcpy(newVetement.image, "");
                        }

                // Enregistrer les données dans le fichier CSV
                enregistrer_vetement_csv("vetement.csv", &newVetement);
                }

            if (event.type == SDL_TEXTINPUT) {
                for (int i = 0; i < 7; i++) {
                    if (fields[i].active) {
                        strncat(fields[i].input, event.text.text, sizeof(fields[i].input) - strlen(fields[i].input) - 1);
                    }
                }
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE) {
                for (int i = 0; i < 7; i++) {
                    if (fields[i].active && strlen(fields[i].input) > 0) {
                        fields[i].input[strlen(fields[i].input) - 1] = '\0';
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < 7; i++) {
            render_field(renderer, font, &fields[i]);
        }
        // Dessiner la barre de recherche
        renderSearchBar(renderer, font, WINDOW_WTH, white);
        render_image_field(renderer, font, &imageRect, imageTexture);
        render_button(renderer, font, "Valider", &buttonRect);

        SDL_RenderPresent(renderer);
    }

    if (imageTexture) {
        SDL_DestroyTexture(imageTexture);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
