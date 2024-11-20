#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    // Créer une surface de texte
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("Erreur de création de surface pour le texte: %s\n", TTF_GetError());
        return;
    }

    // Créer une texture à partir de la surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Erreur de création de texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    // Définir la position du texte
    SDL_Rect dest = {x, y, surface->w, surface->h};
    
    // Copier la texture sur le renderer
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    // Libérer les ressources
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
