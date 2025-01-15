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

// Fonction ajouter vétement dans le csv
void sauvegarder_vetement_csv(const char *chemin_fichier, Vetement *vetement) {
    // Ouvrir le fichier en mode append, pour ajouter des données sans effacer les précédentes
    FILE *fichier = fopen(chemin_fichier, "a");
    if (!fichier) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier : %s\n", chemin_fichier);
        return;
    }

    // Écrire les données du vêtement dans le fichier CSV
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

    // Affichage du titre au-dessus du champ de saisie (centré)
    SDL_Rect titleRect = {field->rect.x + (field->rect.w - titleSurface->w) / 2, field->rect.y - 30, titleSurface->w, titleSurface->h};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);

    if (strlen(field->input) > 0) {
        SDL_Surface *inputSurface = TTF_RenderText_Solid(font, field->input, textColor);
        SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);

        // Affichage du texte saisi à l'intérieur du champ
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

    // Centrer le texte dans le bouton
    SDL_Rect textRect = {buttonRect->x + (buttonRect->w - textSurface->w) / 2, buttonRect->y + (buttonRect->h - textSurface->h) / 2, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// Fonction principale pour ajouter un vêtement
void ajouter_vetement(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Color white = {255, 255, 255, 255};
    const char *img_folder = "img";
    mkdir(img_folder, 0777);

    InputField fields[7] = {
        {{W_WIDTH / 2 - F_WIDTH / 2, 100, F_WIDTH, F_HEIGHT}, "Nom", "", {138, 43, 226, 255}, false},
        {{W_WIDTH / 2 - F_WIDTH / 2, 160, F_WIDTH, F_HEIGHT}, "Categorie", "", {138, 43, 226, 255}, false},
        {{W_WIDTH / 2 - F_WIDTH / 2, 220, F_WIDTH, F_HEIGHT}, "Temperature", "", {138, 43, 226, 255}, false},
        {{W_WIDTH / 2 - F_WIDTH / 2, 280, F_WIDTH, F_HEIGHT}, "Motif", "", {138, 43, 226, 255}, false},
        {{W_WIDTH / 2 - F_WIDTH / 2, 340, F_WIDTH, F_HEIGHT}, "Type", "", {138, 43, 226, 255}, false},
        {{W_WIDTH / 2 - F_WIDTH / 2, 400, F_WIDTH, F_HEIGHT}, "Couleur", "", {138, 43, 226, 255}, false},
        {{W_WIDTH / 2 - F_WIDTH / 2, 460, F_WIDTH, F_HEIGHT}, "Saison", "", {138, 43, 226, 255}, false}
    };

    SDL_Rect buttonValiderRect = {W_WIDTH / 2 - B_WIDTH / 2,580, B_WIDTH, B_HEIGHT};
    SDL_Rect buttonImageRect = {W_WIDTH / 2 - B_WIDTH / 2, 520, B_WIDTH,B_HEIGHT };
    //SDL_Rect buttonRetourRect = {W_WIDTH / 2 - B_WIDTH / 2, 640, B_WIDTH, B_HEIGHT};
    char image_path[200] = "";

    Vetement newVetement = {0};
    SDL_Event event;
    bool running = true;

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
                    fields[i].color = fields[i].active ? (SDL_Color){75, 0, 130, 255} : (SDL_Color){138, 43, 226, 255};
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
                    strcpy(newVetement.image_path, image_path);

                    // Sauvegarder les données dans le fichier CSV
                    sauvegarder_vetement_csv("vetement.csv", &newVetement);
                    //printf("données dans le csv");

                    printf("Nom: %s\n", newVetement.nom);
                    printf("Categorie: %s\n", newVetement.categorie);
                    printf("Temperature: %s\n", newVetement.temperature);
                    printf("Motif: %s\n", newVetement.motif);
                    printf("Type: %s\n", newVetement.type);
                    printf("Couleur: %s\n", newVetement.couleur);
                    printf("Saison: %s\n", newVetement.saison);

                   // Ajouter le vêtement au tableau
                    if (vetement_count < MAX_VETEMENTS) {
                        vetements[vetement_count++] = newVetement;
                        printf("Vêtement ajouté: %s, Total: %d vêtements\n", newVetement.nom, vetement_count);
                    } else {
                        printf("Impossible d'ajouter le vêtement. Tableau plein.\n");

                    }
                    running = false; // Terminer le processus d'ajout du vêtement

                     // Réinitialiser les champs après la validation
                    for (int i = 0; i < 7; i++) {
                         fields[i].input[0] = '\0'; // Vider le contenu de chaque champ
                    }

                    SDL_RenderPresent(renderer); // Rafraîchir l'affichage
                }

                 // Bouton "Importer une image"
                 if (SDL_PointInRect(&(SDL_Point){x, y}, &buttonImageRect)) {
                //     const char *file_path = tinyfd_openFileDialog("Choisissez une image", "", 0, NULL, NULL, 0);
                //     if (file_path && copy_image_to_folder(file_path, img_folder, image_path)) {
                //         printf("Image importée : %s\n", image_path);
                //     }
                // }

                 // Ouvrir une boîte de dialogue pour sélectionner une image
                    const char *filePath = tinyfd_openFileDialog("Choisissez une image", "", 0, NULL, NULL, 0);

                    if (filePath) {
                        // Extraire le nom du fichier à partir du chemin (pour Linux/Mac)
                        const char *fileName = strrchr(filePath, '/');
                        if (!fileName) {
                            // Si c'est Windows, chercher le séparateur '\\'
                            fileName = strrchr(filePath, '\\');
                        }
                        
                        if (fileName) {
                            fileName++;  // Sauter le '/' ou '\\'
                        } else {
                            fileName = filePath;  // Aucun séparateur trouvé, utiliser le chemin complet
                        }

                        // Afficher le nom de l'image (facultatif)
                        printf("Nom du fichier sélectionné : %s\n", fileName);

                        // Sauvegarder le nom de l'image dans une variable, ou une structure
                        strncpy(newVetement.image, fileName, sizeof(newVetement.image) - 1);
                        newVetement.image[sizeof(newVetement.image) - 1] = '\0';  // Assurer la terminaison correcte

                        // Charger l'image dans la texture SDL
                        SDL_Texture *newTexture = import_image(renderer, filePath);
                        if (newTexture) {
                            // Si l'image est correctement chargée, détruire l'ancienne texture et utiliser la nouvelle
                            SDL_DestroyTexture(imageTexture);
                            imageTexture = newTexture;
                        } else {
                            fprintf(stderr, "Erreur lors de l'importation de l'image : %s\n", IMG_GetError());
                        }
                    }
                }

                // Bouton "Retour" : Revenir en arrière
                if (x >= 0 && x <= 800 && y >= 0 && y <= 50) {
                    running = false; // Ferme l'écran sans ajouter de vêtement
                }
            } else if (event.type == SDL_TEXTINPUT) {
                for (int i = 0; i < 7; i++) {
                    if (fields[i].active) {
                        strncat(fields[i].input, event.text.text, sizeof(fields[i].input) - strlen(fields[i].input) - 1);
                    }
                }
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE) {
                for (int i = 0; i < 7; i++) {
                    if (fields[i].active && strlen(fields[i].input) > 0) {
                        fields[i].input[strlen(fields[i].input) - 1] = '\0';
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        // Affichage de la barre de recherche
        render_crea_bar(renderer, font, W_WIDTH, white);

        // Affichage des champs de saisie
        for (int i = 0; i < 7; i++) {
            render_field(renderer, font, &fields[i]);
        }

        // Affichage des boutons
        render_button(renderer, font, &buttonValiderRect, "Valider");
        render_button(renderer, font, &buttonImageRect, "Image");
       // render_button(renderer, font, &buttonRetourRect, "Retour");

        SDL_RenderPresent(renderer);
    }
}



///Début du bon code 
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_ttf.h>
// #include <SDL2/SDL_image.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <string.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include "accessoire.h"
// #include "tinyfiledialogs.h"

// #define MAX_PATH_LENGTH 200

// typedef struct Vetement {
//     char nom[50];
//     char categorie[50];
//     char temperature[20];
//     char motif[50];
//     char type[50];
//     char couleur[30];
//     char saison[20];
//     char image_path[MAX_PATH_LENGTH]; // Chemin complet de l'image
// } Vetement;

// Vetement vetements[MAX_VETEMENTS];
// int vetement_count = 0;

// // Fonction pour copier une image dans le dossier 'img'
// bool copy_image_to_folder(const char *src, const char *dest_folder, char *dest_path) {
//     // Extraire le nom de fichier
//     const char *filename = strrchr(src, '/');
//     if (!filename) filename = src; // Si aucun '/', utiliser src directement
//     else filename++;               // Sauter le '/'

//     snprintf(dest_path, MAX_PATH_LENGTH, "%s/%s", dest_folder, filename);

//     FILE *src_file = fopen(src, "rb");
//     if (!src_file) {
//         fprintf(stderr, "Erreur ouverture fichier source : %s\n", src);
//         return false;
//     }

//     FILE *dest_file = fopen(dest_path, "wb");
//     if (!dest_file) {
//         fprintf(stderr, "Erreur ouverture fichier destination : %s\n", dest_path);
//         fclose(src_file);
//         return false;
//     }

//     char buffer[4096];
//     size_t bytes;
//     while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
//         fwrite(buffer, 1, bytes, dest_file);
//     }

//     fclose(src_file);
//     fclose(dest_file);

//     return true;
// }

// // Fonction pour ajouter un vêtement
// void ajouter_vetement(SDL_Renderer *renderer, TTF_Font *font) {
//     // Créer un dossier pour les images si inexistant
//     const char *img_folder = "img";
//     mkdir(img_folder, 0777);

//     Vetement newVetement;
//     memset(&newVetement, 0, sizeof(Vetement));

//     // Interface pour saisir les informations (simplifié ici)
//     printf("Nom : "); scanf("%49s", newVetement.nom);
//     printf("Catégorie : "); scanf("%49s", newVetement.categorie);
//     printf("Température : "); scanf("%19s", newVetement.temperature);
//     printf("Motif : "); scanf("%49s", newVetement.motif);
//     printf("Type : "); scanf("%49s", newVetement.type);
//     printf("Couleur : "); scanf("%29s", newVetement.couleur);
//     printf("Saison : "); scanf("%19s", newVetement.saison);

//     // Importer l'image
//     const char *file_path = tinyfd_openFileDialog("Choisissez une image", "", 0, NULL, NULL, 0);
//     if (file_path) {
//         if (copy_image_to_folder(file_path, img_folder, newVetement.image_path)) {
//             printf("Image importée : %s\n", newVetement.image_path);
//         } else {
//             fprintf(stderr, "Erreur lors de l'importation de l'image.\n");
//             strcpy(newVetement.image_path, ""); // Pas d'image associée
//         }
//     }

//     // Ajouter le vêtement à la liste
//     if (vetement_count < MAX_VETEMENTS) {
//         vetements[vetement_count++] = newVetement;
//         printf("Vêtement ajouté avec succès !\n");
//     } else {
//         fprintf(stderr, "Limite maximale de vêtements atteinte !\n");
//     }
// }

// // Fonction pour afficher les vêtements
// void afficher_vetements() {
//     for (int i = 0; i < vetement_count; i++) {
//         printf("Vêtement %d:\n", i + 1);
//         printf("  Nom : %s\n", vetements[i].nom);
//         printf("  Catégorie : %s\n", vetements[i].categorie);
//         printf("  Température : %s\n", vetements[i].temperature);
//         printf("  Motif : %s\n", vetements[i].motif);
//         printf("  Type : %s\n", vetements[i].type);
//         printf("  Couleur : %s\n", vetements[i].couleur);
//         printf("  Saison : %s\n", vetements[i].saison);
//         printf("  Image : %s\n", vetements[i].image_path[0] ? vetements[i].image_path : "Pas d'image");
//     }
// }
///Fin du bon code 
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_ttf.h>
// #include <SDL2/SDL_image.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <string.h>
// #include "barre_nav.h"
// #include "search_bar.h"
// #include "ajouter_vêtement.h"
// #include "tinyfiledialogs.h"

// // Fonction pour afficher un champ d'entrée avec son titre et son contenu
// void render_field(SDL_Renderer *renderer, TTF_Font *font, InputField *field) {
//     // Dessiner le titre du champ
//     SDL_Color textColor = {0, 0, 0, 255}; // Noir
//     SDL_Surface *titleSurface = TTF_RenderText_Solid(font, field->title, textColor);
//     SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);

//     int titleWidth = 0, titleHeight = 0;
//     TTF_SizeText(font, field->title, &titleWidth, &titleHeight);

//     SDL_Rect titleRect = {
//         field->rect.x + (field->rect.w - titleWidth) / 2,
//         field->rect.y - titleHeight - 10,
//         titleWidth,
//         titleHeight
//     };
//     SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

//     SDL_FreeSurface(titleSurface);
//     SDL_DestroyTexture(titleTexture);

//     // Dessiner le champ
//     SDL_SetRenderDrawColor(renderer, field->color.r, field->color.g, field->color.b, 255);
//     SDL_RenderFillRect(renderer, &field->rect);

//     // Dessiner le contenu du champ (texte saisi)
//     if (field->input[0] != '\0') { // Si le champ contient du texte
//         SDL_Surface *inputSurface = TTF_RenderText_Solid(font, field->input, textColor);
//         SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);

//         SDL_Rect inputRect = {
//             field->rect.x + 10,  // Marges intérieures pour le texte
//             field->rect.y + (field->rect.h - titleHeight) / 2,
//             inputSurface->w,
//             inputSurface->h
//         };
//         SDL_RenderCopy(renderer, inputTexture, NULL, &inputRect);

//         SDL_FreeSurface(inputSurface);
//         SDL_DestroyTexture(inputTexture);
//     }
// }

// // Fonction pour afficher un bouton
// void render_button(SDL_Renderer *renderer, TTF_Font *font, const char *label, SDL_Rect *buttonRect) {
//     // Dessiner le bouton
//     SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Vert
//     SDL_RenderFillRect(renderer, buttonRect);

//     // Dessiner le texte sur le bouton
//     SDL_Color textColor = {0, 0, 0, 255}; // Noir
//     SDL_Surface *textSurface = TTF_RenderText_Solid(font, label, textColor);
//     SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

//     int textWidth = 0, textHeight = 0;
//     TTF_SizeText(font, label, &textWidth, &textHeight);

//     SDL_Rect textRect = {
//         buttonRect->x + (buttonRect->w - textWidth) / 2,
//         buttonRect->y + (buttonRect->h - textHeight) / 2,
//         textWidth,
//         textHeight
//     };
//     SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

//     SDL_FreeSurface(textSurface);
//     SDL_DestroyTexture(textTexture);
// }

// // Fonction pour importer une image
// SDL_Texture* import_image(SDL_Renderer *renderer, const char *file_path) {
//     SDL_Surface *imageSurface = IMG_Load(file_path);
//     if (!imageSurface) {
//         fprintf(stderr, "Erreur chargement image : %s\n", IMG_GetError());
//         return NULL;
//     }

//     SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
//     SDL_FreeSurface(imageSurface);

//     return imageTexture;
// }

// // Fonction pour dessiner un champ spécifique pour l'image
// void render_image_field(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect *rect, SDL_Texture *imageTexture) {
//     // Dessiner le cadre en violet foncé
//     SDL_SetRenderDrawColor(renderer, 75, 0, 130, 255); // Violet foncé
//     SDL_RenderFillRect(renderer, rect);

//     if (imageTexture) {
//         // Si une image est disponible, afficher une miniature
//         SDL_Rect imageRect = {
//             rect->x + 10, rect->y + 5,
//             rect->w - 20, rect->h - 10
//         };
//         SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
//     } else {
//         // Si aucune image, afficher le texte "Importer une image" en blanc
//         SDL_Color textColor = {255, 255, 255, 255}; // Blanc
//         SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Importer une image", textColor);
//         SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

//         int textWidth, textHeight;
//         TTF_SizeText(font, "Importer une image", &textWidth, &textHeight);

//         SDL_Rect textRect = {
//             rect->x + (rect->w - textWidth) / 2,
//             rect->y + (rect->h - textHeight) / 2,
//             textWidth,
//             textHeight
//         };
//         SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

//         SDL_FreeSurface(textSurface);
//         SDL_DestroyTexture(textTexture);
//     }
// }


// void ajouter_vetement() {
//     SDL_Color white = {255,255,255,255};
//     SDL_Window *fenetre = SDL_CreateWindow(
//         "Ajouter un vêtement",
//         SDL_WINDOWPOS_CENTERED,
//         SDL_WINDOWPOS_CENTERED,
//         WINDOW_WTH,
//         WINDOW_HEIGHT,
//         SDL_WINDOW_SHOWN
//     );

//     if (!fenetre) {
//         fprintf(stderr, "Erreur création fenêtre secondaire : %s\n", SDL_GetError());
//         return;
//     }

//     SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//     if (!renderer) {
//         fprintf(stderr, "Erreur création renderer secondaire : %s\n", SDL_GetError());
//         SDL_DestroyWindow(fenetre);
//         return;
//     }

//     if (TTF_Init() != 0 || IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
//         fprintf(stderr, "Erreur initialisation des bibliothèques : %s\n", TTF_GetError());
//         SDL_DestroyRenderer(renderer);
//         SDL_DestroyWindow(fenetre);
//         return;
//     }

//     TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf", 18);
//     if (!font) {
//         fprintf(stderr, "Erreur chargement police : %s\n", TTF_GetError());
//         SDL_DestroyRenderer(renderer);
//         SDL_DestroyWindow(fenetre);
//         TTF_Quit();
//         IMG_Quit();
//         return;
//     }

//     InputField fields[7];
//     const char *titles[] = {"Nom", "Categorie", "Temperature", "Motif", "Type", "Couleur", "Saison"};
//     SDL_Color defaultColor = {255, 0, 0, 255};
//     SDL_Color activeColor = {0, 255, 0, 255};

//     for (int i = 0; i < 7; i++) {
//         fields[i].rect.x = (WINDOW_WTH - FIELD_WIDTH) / 2;
//         fields[i].rect.y = 100 + i * (FIELD_HEIGHT + FIELD_MARGIN);
//         fields[i].rect.w = FIELD_WIDTH;
//         fields[i].rect.h = FIELD_HEIGHT;
//         fields[i].color = defaultColor;
//         fields[i].title = titles[i];
//         fields[i].active = false;
//         fields[i].input[0] = '\0';
//     }
//     // Définition du champ "Importer une image"
//     SDL_Rect imageRect = {
//         (WINDOW_WTH - FIELD_WIDTH) / 2,
//         100 + 7 * (FIELD_HEIGHT + FIELD_MARGIN),
//         FIELD_WIDTH,
//         FIELD_HEIGHT
//     };

//     // Définition du bouton "Valider" avec un espace ajouté
//     SDL_Rect buttonRect = {
//         (WINDOW_WTH - BUTTON_WTH ) / 2,
//         100 + 8 * (FIELD_HEIGHT + FIELD_MARGIN) + 20, // 20px d'espace supplémentaire
//         BUTTON_WTH ,
//         BUTTON_HEIGHT
//     };

//     SDL_Texture *imageTexture = NULL;
    
//     SDL_StartTextInput();

//     Vetement newVetement;
//     bool running = true;
//     SDL_Event event;

//     while (running) {
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_QUIT) {
//                 running = false;
//             }

//             if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_FINGERDOWN) {
//                 int x = event.type == SDL_MOUSEBUTTONDOWN ? event.button.x : (int)(event.tfinger.x * WINDOW_WTH);
//                 int y = event.type == SDL_MOUSEBUTTONDOWN ? event.button.y : (int)(event.tfinger.y * WINDOW_HEIGHT);

//                 for (int i = 0; i < 7; i++) {
//                     if (x >= fields[i].rect.x && x <= fields[i].rect.x + fields[i].rect.w &&
//                         y >= fields[i].rect.y && y <= fields[i].rect.y + fields[i].rect.h) {
//                         fields[i].active = true;
//                         fields[i].color = activeColor;
//                     } else {
//                         fields[i].active = false;
//                         fields[i].color = defaultColor;
//                     }
//                 }

//                 if (x >= imageRect.x && x <= imageRect.x + imageRect.w &&
//                     y >= imageRect.y && y <= imageRect.y + imageRect.h) {
//                     // Ouvrir une boîte de dialogue pour sélectionner une image
//                     const char *filePath = tinyfd_openFileDialog(
//                         "Choisissez une image", "", 0, NULL, NULL, 0
//                     );

//                     if (filePath) {
//                         SDL_Texture *newTexture = import_image(renderer, filePath);
//                         if (newTexture) {
//                             SDL_DestroyTexture(imageTexture);
//                             imageTexture = newTexture;
//                         } else {
//                             fprintf(stderr, "Erreur lors de l'importation de l'image : %s\n", IMG_GetError());
//                         }
//                     }
//                 }

//                 if (x >= buttonRect.x && x <= buttonRect.x + buttonRect.w &&
//                     y >= buttonRect.y && y <= buttonRect.y + buttonRect.h) {
//                     strcpy(newVetement.nom, fields[0].input);
//                     strcpy(newVetement.categorie, fields[1].input);
//                     strcpy(newVetement.temperature, fields[2].input);
//                     strcpy(newVetement.motif, fields[3].input);
//                     strcpy(newVetement.type, fields[4].input);
//                     strcpy(newVetement.couleur, fields[5].input);
//                     strcpy(newVetement.saison, fields[6].input);

//                     printf("Nom: %s\n", newVetement.nom);
//                     printf("Categorie: %s\n", newVetement.categorie);
//                     printf("Temperature: %s\n", newVetement.temperature);
//                     printf("Motif: %s\n", newVetement.motif);
//                     printf("Type: %s\n", newVetement.type);
//                     printf("Couleur: %s\n", newVetement.couleur);
//                     printf("Saison: %s\n", newVetement.saison);
//                 }
//             }

//             if (event.type == SDL_TEXTINPUT) {
//                 for (int i = 0; i < 7; i++) {
//                     if (fields[i].active) {
//                         strncat(fields[i].input, event.text.text, sizeof(fields[i].input) - strlen(fields[i].input) - 1);
//                     }
//                 }
//             }

//             if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE) {
//                 for (int i = 0; i < 7; i++) {
//                     if (fields[i].active && strlen(fields[i].input) > 0) {
//                         fields[i].input[strlen(fields[i].input) - 1] = '\0';
//                     }
//                 }
//             }
//         }

//         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//         SDL_RenderClear(renderer);

//         for (int i = 0; i < 7; i++) {
//             render_field(renderer, font, &fields[i]);
//         }
//         // Dessiner la barre de recherche
//         AppState *currentState;
//         renderSearchBar(renderer, font, WINDOW_WTH, white,currentState);
//         render_image_field(renderer, font, &imageRect, imageTexture);
//         render_button(renderer, font, "Valider", &buttonRect);

//         SDL_RenderPresent(renderer);
//     }

//     if (imageTexture) {
//         SDL_DestroyTexture(imageTexture);
//     }

//     TTF_CloseFont(font);
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(fenetre);
//     TTF_Quit();
//     IMG_Quit();
//     SDL_Quit();
// }
