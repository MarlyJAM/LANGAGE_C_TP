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
#include "ajouter_vêtement.h"
#include "crea_bar.h"

Vetement vetements[MAX_VETEMENTS];
int vetement_count = 0;

// Fonction pour copier une image
bool copy_image_to_folder(const char *src, const char *dest_folder, char *dest_path) {
    const char *filename = strrchr(src, '/');
    if (!filename) filename = src;
    else filename++;

    snprintf(dest_path, 200, "%s/%s", dest_folder, filename);

    FILE *src_file = fopen(src, "rb");
    if (!src_file) return false;

    FILE *dest_file = fopen(dest_path, "wb");
    if (!dest_file) {
        fclose(src_file);
        return false;
    }

    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
        fwrite(buffer, 1, bytes, dest_file);
    }

    fclose(src_file);
    fclose(dest_file);
    return true;
}

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

// Fonction ajouter vêtement dans le csv
void sauvegarder_vetement_csv(const char *chemin_fichier, Vetement *vetement) {
    FILE *fichier = fopen(chemin_fichier, "a");
    if (!fichier) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier : %s\n", chemin_fichier);
        return;
    }

    fprintf(fichier, "%s,%s,%s,%s,%s,%s,%s,%s\n",
            vetement->nom,
            vetement->categorie,
            vetement->temperature,
            vetement->motif,
            vetement->type,
            vetement->couleur,
            vetement->saison,
            vetement->image);
    
    fclose(fichier);
}

// Fonction pour rendre un champ avec son nom (au-dessus du champ, sauf pour "Importer une image")
void render_field(SDL_Renderer *renderer, TTF_Font *font, InputField *field) {
    SDL_SetRenderDrawColor(renderer, field->color.r, field->color.g, field->color.b, 255);
    SDL_RenderFillRect(renderer, &field->rect);

    SDL_Color textColor = {0, 0, 0, 255}; // Texte en noir
    SDL_Surface *titleSurface = TTF_RenderText_Solid(font, field->title, textColor);
    SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);

    SDL_Rect titleRect = {field->rect.x + (field->rect.w - titleSurface->w) / 2, field->rect.y - 30, titleSurface->w, titleSurface->h};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);

    if (strlen(field->input) > 0) {
        SDL_Surface *inputSurface = TTF_RenderText_Solid(font, field->input, textColor);
        SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);

        SDL_Rect inputRect = {field->rect.x + 10, field->rect.y + 10, inputSurface->w, inputSurface->h};
        SDL_RenderCopy(renderer, inputTexture, NULL, &inputRect);

        SDL_FreeSurface(inputSurface);
        SDL_DestroyTexture(inputTexture);
    }
}

// Fonction pour afficher le texte sur les boutons
void render_button(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect *buttonRect, const char *text) {
    SDL_SetRenderDrawColor(renderer, 75, 0, 130, 255); // Couleur pour les boutons (violet)
    SDL_RenderFillRect(renderer, buttonRect);

    SDL_Color textColor = {255, 255, 255, 255}; // Texte en blanc
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = {buttonRect->x + (buttonRect->w - textSurface->w) / 2, buttonRect->y + (buttonRect->h - textSurface->h) / 2, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// Fonction pour afficher un bouton avec une icône
void render_image_button(SDL_Renderer *renderer, SDL_Texture *iconTexture, SDL_Rect buttonRect) {
    if (iconTexture) {
        SDL_RenderCopy(renderer, iconTexture, NULL, &buttonRect);
    }
}

// Fonction pour ajouter un vêtement avec l'option d'image
void ajouter_vetement(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Texture *imageTexture = NULL;
    SDL_Color white = {255, 255, 255, 255};
    const char *img_folder = "img";
    mkdir(img_folder, 0777);

    SDL_Texture *backgroundTexture = chargerimage("background_welcome.png", renderer);

    // Calcul des dimensions et position des champs de saisie et des boutons pour centrer
    int field_width = F_WIDTH + 40;
    int field_height = F_HEIGHT;
    int start_y = 100;
    int spacing = 70;

    InputField fields[7] = {
        {{(W_WIDTH - field_width) / 2, start_y, field_width, field_height}, "Nom", "", {138, 43, 226, 255}, false},
        {{(W_WIDTH - field_width) / 2, start_y + spacing, field_width, field_height}, "Categorie", "", {138, 43, 226, 255}, false},
        {{(W_WIDTH - field_width) / 2, start_y + 2 * spacing, field_width, field_height}, "Temperature", "", {138, 43, 226, 255}, false},
        {{(W_WIDTH - field_width) / 2, start_y + 3 * spacing, field_width, field_height}, "Motif", "", {138, 43, 226, 255}, false},
        {{(W_WIDTH - field_width) / 2, start_y + 4 * spacing, field_width, field_height}, "Type", "", {138, 43, 226, 255}, false},
        {{(W_WIDTH - field_width) / 2, start_y + 5 * spacing, field_width, field_height}, "Couleur", "", {138, 43, 226, 255}, false},
        {{(W_WIDTH - field_width) / 2, start_y + 6 * spacing, field_width, field_height}, "Saison", "", {138, 43, 226, 255}, false}
    };

    // Positionnement du bouton "Valider"
    SDL_Rect buttonValiderRect = {(W_WIDTH - B_WIDTH) / 2, 720, B_WIDTH, B_HEIGHT};

    // Positionnement du bouton "Importer une image"
    SDL_Rect buttonImageRect = {(W_WIDTH - 200) / 2, 600, 200, 100}; // Centré avec une taille de 200x100

    char image_path[200] = "";

    Vetement newVetement = {0};
    SDL_Event event;
    bool running = true;

    SDL_Texture *importImageIcon = chargerimage("camera_icon.png", renderer);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                // Activation des champs de saisie
                for (int i = 0; i < 7; i++) {
                    fields[i].active = SDL_PointInRect(&(SDL_Point){x, y}, &fields[i].rect);
                    fields[i].color = fields[i].active ? (SDL_Color){255, 255, 255, 255} : (SDL_Color){227, 177, 227, 1};
                }

                // Bouton "Valider"
                if (SDL_PointInRect(&(SDL_Point){x, y}, &buttonValiderRect)) {
                    strcpy(newVetement.nom, fields[0].input);
                    strcpy(newVetement.categorie, fields[1].input);
                    strcpy(newVetement.temperature, fields[2].input);
                    strcpy(newVetement.motif, fields[3].input);
                    strcpy(newVetement.type, fields[4].input);
                    strcpy(newVetement.couleur, fields[5].input);
                    strcpy(newVetement.saison, fields[6].input);
                    strcpy(newVetement.image, image_path);

                    sauvegarder_vetement_csv("vetement.csv", &newVetement);
                    printf("Vêtement ajouté: %s, Total: %d vêtements\n", newVetement.nom, vetement_count);

                    if (vetement_count < MAX_VETEMENTS) {
                        vetements[vetement_count++] = newVetement;
                    } else {
                        printf("Impossible d'ajouter le vêtement. Tableau plein.\n");
                    }
                    running = false;
                }

                // Bouton "Importer une image"
                if (SDL_PointInRect(&(SDL_Point){x, y}, &buttonImageRect)) {
                    const char *filePath = tinyfd_openFileDialog("Choisissez une image", "", 0, NULL, NULL, 0);
                    if (filePath) {
                        strncpy(image_path, filePath, sizeof(image_path) - 1);
                        image_path[sizeof(image_path) - 1] = '\0';
                    }
                }
            } else if (event.type == SDL_TEXTINPUT) {
                for (int i = 0; i < 7; i++) {
                    if (fields[i].active) {
                        strncat(fields[i].input, event.text.text, sizeof(fields[i].input) - strlen(fields[i].input) - 1);
                    }
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    for (int i = 0; i < 7; i++) {
                        if (fields[i].active && strlen(fields[i].input) > 0) {
                            fields[i].input[strlen(fields[i].input) - 1] = '\0';
                        }
                    }
                }
            }
            // Gestion de la barre de recherche pour revenir à l'accueil
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;

                    // Vérifiez si le clic est dans la zone de la barre de recherche
                    if (x >= 0 && x <= W_WIDTH && y >= 0 && y <= 50) { // Barre de recherche
                        printf("Retour à l'accueil\n");
                        running = false;
                        
                    }
            }
        }

        SDL_RenderClear(renderer);
        
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        // Affichage de la barre de recherche
        render_crea_bar(renderer, font, W_WIDTH, white);
        for (int i = 0; i < 7; i++) {
            render_field(renderer, font, &fields[i]);
        }

        render_button(renderer, font, &buttonValiderRect, "Valider");
        render_image_button(renderer, importImageIcon, buttonImageRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60); // Limite de 60 FPS
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(importImageIcon);
}


SDL_Texture* chargerimage(const char *fichier, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(fichier);
    if (!surface) {
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
