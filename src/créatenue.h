#ifndef CREATE_TENU_H
#define CREATE_TENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "accessoire.h"

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 800
#ifndef BUTTON_WIDTH
#define BUTTON_WIDTH 210
#endif
#ifndef BUTTON_HEIGHT 
#define BUTTON_HEIGHT 50
#endif


// Prototypes
int init(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font);
void afficher_texte(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y, SDL_Color couleur);
void afficher_tenue(SDL_Renderer *renderer, TTF_Font *font, Tenue *tenue);
void generate_tenue(Tenue *tenue);
SDL_Texture* charger_image(const char *fichier, SDL_Renderer *renderer);
void sauvegarder_tenues_csv(const char *fichier) ;
int render_createnue() ;

#endif
