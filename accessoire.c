#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h> 
#include <stdio.h>
#include <stdlib.h>
#include "accessoire.h"
#include <time.h>
#include <string.h>

char hauts[MAX_VETEMENTS][MAX_LEN] = {"T-shirt noir", "T-shirt blanc", "Pull", "Chemise"};
char pantalons[MAX_VETEMENTS][MAX_LEN] = {"Jean", "Pantalon noir", "Jupe", "Short"};
char chaussures[MAX_VETEMENTS][MAX_LEN] = {"Baskets blanches", "Chaussures noires", "Sandales", "Bottes"};
int num_hauts = 4, num_pantalons = 4, num_chaussures = 4;
Tenue tenue_affichee;
Tenue tenues_sauvegardees[MAX_TENUES];
int nombre_tenues_sauvegardees = 0;

