#pragma once
#include <SDL.h>
struct characterBitmaps {
        SDL_Surface* surfaces[128];
        SDL_Texture* textures[128];
};
bool loadCharacterBitmaps(characterBitmaps* chars, SDL_Renderer* renderer);
void drawText(const char* text, int x, int y, characterBitmaps* chars, SDL_Renderer* renderer);
void cleanupCharacterBitmaps(characterBitmaps* chars);
