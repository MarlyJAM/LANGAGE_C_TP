#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>  // Pour charger les images
#include <stdio.h>
#include <stdlib.h>
#include "créatenue.h"
#include <time.h>
#include <string.h>
#include "crea_bar.h"
#include "ajouter_vêtement.h"


// Déclarer une texture pour le fond
SDL_Texture *background_texture = NULL;

// Initialisation SDL
int init(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() == -1) {
        printf("Erreur SDL_ttf : %s\n", TTF_GetError());
        return 0;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erreur SDL_image : %s\n", IMG_GetError());
        return 0;
    }

    *window = SDL_CreateWindow("Générateur de Tenues", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("Erreur création fenêtre : %s\n", SDL_GetError());
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer) {
        printf("Erreur création renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        return 0;
    }

    *font = TTF_OpenFont("/usr/share/fonts/truetype/msttcorefonts/Georgia_Bold_Italic.ttf", 24);
    if (!*font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        return 0;
    }
    // Charger l'image de fond
    background_texture = charger_image("background_welcome.png", *renderer);  // Remplace "background.png" par ton fichier de fond
    return 1;
}

// Fonction pour afficher un texte
void afficher_texte(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y, SDL_Color couleur) {
    // Vérifier si le texte est valide
    if (texte == NULL || strlen(texte) == 0) {
        return; // Ne rien faire si le texte est vide ou nul
    }

    SDL_Surface *surface = TTF_RenderText_Blended(font, texte, couleur);
    if (!surface) {
        fprintf(stderr, "Erreur lors de la création de la surface pour le texte : %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        fprintf(stderr, "Erreur lors de la création de la texture : %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}




// Fonction pour afficher une tenue avec icônes
void afficher_tenue(SDL_Renderer *renderer, TTF_Font *font, Tenue *tenue) {
    SDL_Color noir = {0, 0, 0, 255};
    int x_pos = 150;
    int l_icon = 175;


    // Affichage de l'icône du haut et du texte
    SDL_Rect dst_haut = {x_pos, 100, l_icon, l_icon}; // Taille et position de l'icône
    SDL_RenderCopy(renderer, tenue->haut_icon, NULL, &dst_haut);
    //afficher_texte(renderer, font, tenue->haut, x_pos + 60, 100, noir);

    // Affichage de l'icône du pantalon et du texte
    SDL_Rect dst_pantalon = {x_pos, 275 , l_icon, l_icon}; // Taille et position de l'icône
    SDL_RenderCopy(renderer, tenue->pantalon_icon, NULL, &dst_pantalon);
    //afficher_texte(renderer, font, tenue->pantalon, x_pos + 60, 150, noir);

    // Affichage de l'icône de la chaussure et du texte
    SDL_Rect dst_chaussure = {x_pos, 450, l_icon, l_icon}; // Taille et position de l'icône
    SDL_RenderCopy(renderer, tenue->chaussure_icon, NULL, &dst_chaussure);
    //afficher_texte(renderer, font, tenue->chaussure, x_pos + 60, 200, noir);
}


// Fonction principale
int render_createnue() {
    SDL_Color white = {255, 255, 255, 255};
    srand(time(NULL));

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *font = NULL;

    if (!init(&window, &renderer, &font)) {
        return 1;
    }

    int quit = 0;
    SDL_Event e;

    // Définition des boutons
    SDL_Rect bouton_generer = {WINDOW_WIDTH / 2 - BUTTON_WIDTH / 2, WINDOW_HEIGHT - 120, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect bouton_sauvegarder = {WINDOW_WIDTH / 2 - BUTTON_WIDTH / 2, WINDOW_HEIGHT - 60, BUTTON_WIDTH, BUTTON_HEIGHT};

    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    // Charger les icônes
    const char* icone_a_charger = NULL;
    const char* icone_a_charger1 = NULL;
    const char* icone_a_charger2 = NULL;
    //tenue_affichee.haut_icon = charger_image(icone_a_charger, renderer);
    tenue_affichee.haut_icon = charger_image("icon_tshirt.png", renderer);
    tenue_affichee.pantalon_icon = charger_image("icon_jean.png", renderer);
    tenue_affichee.chaussure_icon = charger_image("icon_basket.png", renderer);

    // Initialiser la première tenue affichée
    generate_tenue(&tenue_affichee);


    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                // Gestion du clic sur le bouton "Générer"
                if (x >= bouton_generer.x && x <= bouton_generer.x + BUTTON_WIDTH &&
                    y >= bouton_generer.y && y <= bouton_generer.y + BUTTON_HEIGHT) {
                    printf("bouton générer est cliqué");
                    generate_tenue(&tenue_affichee);



                    // Sélection aléatoire d'un haut
                    int index_aleatoire = rand() % MAX_VETEMENTS;
                    int index_aleatoire1 = rand() % MAX_VETEMENTS;
                    int index_aleatoire2 = rand() % MAX_VETEMENTS;
                    const char* icone_a_charger = hauts[index_aleatoire];
                    const char* icone_a_charger1 = pantalons[index_aleatoire1];
                    const char* icone_a_charger2 = chaussures[index_aleatoire2];


                    printf("Index aléatoire : %d\n", index_aleatoire);
                    printf("Haut choisi : %s\n", hauts[index_aleatoire]);

                    // Assignation du nom du haut à la structure
                    snprintf(tenue_affichee.haut, MAX_LEN, "%s", icone_a_charger);
                    printf("Haut sélectionné : %s\n", tenue_affichee.haut);

                    // Assignation du nom du pantalon à la structure
                    snprintf(tenue_affichee.pantalon, MAX_LEN, "%s", icone_a_charger1);
                    printf("Pantalon sélectionné : %s\n", tenue_affichee.pantalon);

                    // Assignation du nom des chaussures à la structure
                    snprintf(tenue_affichee.chaussure, MAX_LEN, "%s", icone_a_charger2);
                    printf("Chaussure sélectionné : %s\n", tenue_affichee.chaussure);

                    // Chargement de l'icône du haut
                    if (tenue_affichee.haut_icon) {
                        SDL_DestroyTexture(tenue_affichee.haut_icon); // Libérer la texture précédente
                    }

                    //printf("Le fichier à charger est : %s\n", icone_a_charger);

                    tenue_affichee.haut_icon = charger_image(icone_a_charger, renderer);
                    if (!tenue_affichee.haut_icon) {
                        printf("Erreur lors du chargement de l'image : %s\n", icone_a_charger);
                    }

                    // Chargement de l'icône du milieu
                    if (tenue_affichee.pantalon_icon) {
                        SDL_DestroyTexture(tenue_affichee.pantalon_icon); // Libérer la texture précédente
                    }

                    //printf("Le fichier à charger est : %s\n", icone_a_charger1);

                    tenue_affichee.pantalon_icon = charger_image(icone_a_charger1, renderer);
                    if (!tenue_affichee.pantalon_icon) {
                        printf("Erreur lors du chargement de l'image : %s\n", icone_a_charger1);
                    }

                      // Chargement de l'icône du bas
                    if (tenue_affichee.chaussure_icon) {
                        SDL_DestroyTexture(tenue_affichee.chaussure_icon); // Libérer la texture précédente
                    }

                    //printf("Le fichier à charger est : %s\n", icone_a_charger2);

                    tenue_affichee.chaussure_icon = charger_image(icone_a_charger2, renderer);
                    if (!tenue_affichee.chaussure_icon) {
                        printf("Erreur lors du chargement de l'image : %s\n", icone_a_charger2);
                    }

                    


                }

                // Gestion du clic sur le bouton "Sauvegarder"
                if (x >= bouton_sauvegarder.x && x <= bouton_sauvegarder.x + BUTTON_WIDTH &&
                    y >= bouton_sauvegarder.y && y <= bouton_sauvegarder.y + BUTTON_HEIGHT) {
                    if (nombre_tenues_sauvegardees < MAX_TENUES) {
                        tenues_sauvegardees[nombre_tenues_sauvegardees++] = tenue_affichee;
                    }
                }
                // // Gestion de la barre de recherche pour revenir à l'accueil
                // if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                //     int x = e.button.x;
                //     int y = e.button.y;

                //     // Vérifiez si le clic est dans la zone de la barre de recherche
                //     if (x >= 0 && x <= WINDOW_WIDTH && y >= 0 && y <= 50) { // Barre de recherche
                //         printf("Retour à l'accueil\n");
                //         ajouter_vetement(renderer,font);
                        
                //     }
                // }
            }
        }

        // Dessiner l'interface
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        // Afficher l'image de fond
        SDL_Rect background_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, background_texture, NULL, &background_rect);
        // Affichage de la barre de recherche
        render_crea_bar(renderer, font, WINDOW_WIDTH, white);
        // Afficher la tenue
        afficher_tenue(renderer, font, &tenue_affichee);

        // Dessiner les boutons
        SDL_SetRenderDrawColor(renderer, 200, 0, 255, 255);
        SDL_RenderFillRect(renderer, &bouton_generer);
        SDL_RenderFillRect(renderer, &bouton_sauvegarder);

        // Afficher le texte des boutons
        SDL_Color blanc = {255, 255, 255, 255};
        afficher_texte(renderer, font, "Generer Tenue", bouton_generer.x + 20, bouton_generer.y + 10, blanc);
        afficher_texte(renderer, font, "Sauvegarder", bouton_sauvegarder.x + 20, bouton_sauvegarder.y + 10, blanc);

        SDL_RenderPresent(renderer);
    }

    // Sauvegarde des tenues dans un fichier CSV
    sauvegarder_tenues_csv("tenues.csv");

    // Nettoyage
    SDL_DestroyTexture(tenue_affichee.haut_icon);
    SDL_DestroyTexture(background_texture); // Libère la texture du fond
   
}
SDL_Texture* charger_image(const char *fichier, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(fichier);
    printf("Tentative de chargement de l'image : %s\n", fichier);
    if (!surface) {
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void generate_tenue(Tenue *tenue) {
    // Choix aléatoire des éléments de la tenue
    const char *haut = hauts[rand() % 4];
    const char *pantalon = pantalons[rand() % 4];
    const char *chaussure = chaussures[rand() % 4];

    // Calculer la longueur totale des chaînes
    size_t total_length = strlen(haut) + strlen(pantalon) + strlen(chaussure) + 2; // +2 pour les virgules et espaces

    // Vérifier si la taille totale ne dépasse pas MAX_LEN
    if (total_length < MAX_LEN) {
        snprintf(tenue->description, MAX_LEN, "%s, %s, %s", haut, pantalon, chaussure);
    } else {
        // Si la chaîne est trop longue, tronquer chaque élément pour respecter la taille MAX_LEN
        snprintf(tenue->description, MAX_LEN, 
                 "%s, %s, %s", 
                 (strlen(haut) < MAX_LEN / 3 ? haut : "Trop long"), 
                 (strlen(pantalon) < MAX_LEN / 3 ? pantalon : "Trop long"), 
                 (strlen(chaussure) < MAX_LEN / 3 ? chaussure : "Trop long"));
    }
}


void sauvegarder_tenues_csv(const char *fichier) {
    FILE *fp = fopen(fichier, "w");
    if (!fp) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier : %s\n", fichier);
        return;
    }
    for (int i = 0; i < nombre_tenues_sauvegardees; i++) {
        fprintf(fp, "%s\n", tenues_sauvegardees[i].description);
    }
    fclose(fp);
}
