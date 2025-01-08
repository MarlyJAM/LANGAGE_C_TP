// vetements.h

#ifndef VETEMENTS_H
#define VETEMENTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ajouter_vêtement.h"  // Inclure le fichier ajouter_vêtement.h pour accéder aux structures de données

// Déclaration de la fonction afficher_vetements
// Cette fonction parcourt le tableau des vêtements et les affiche sur l'écran
void afficher_vetements(SDL_Renderer *renderer, TTF_Font *font);
int renderer_vet();

#endif // VETEMENTS_H
