#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define BUTTON_WIDTH 400
#define BUTTON_HEIGHT 80
#define BUTTON_MARGIN 20

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    const char *text;
} Button;

bool is_point_in_rect(int x, int y, SDL_Rect *rect) {
    return x >= rect->x && x <= rect->x + rect->w && y >= rect->y && y <= rect->y + rect->h;
}

void render_button(SDL_Renderer *renderer, TTF_Font *font, Button *button) {
    // Dessiner le rectangle
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, 255);
    SDL_RenderFillRect(renderer, &button->rect);

    // Dessiner le texte
    SDL_Color textColor = {0, 0, 0, 255}; // Noir
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, button->text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    int textWidth = 0, textHeight = 0;
    TTF_SizeText(font, button->text, &textWidth, &textHeight);

    SDL_Rect textRect = {
        button->rect.x + (button->rect.w - textWidth) / 2,
        button->rect.y + (button->rect.h - textHeight) / 2,
        textWidth,
        textHeight
    };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}