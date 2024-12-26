#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    const char *text;
} Button;
