#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "blueDot.h"
#include "snake.h"
#include "game_state.h"
bool initializeBlueDot(BlueDot* blueDot, GameState* game) {
    FILE* debugFile = fopen("debug.txt", "a");
    fprintf(debugFile, "\n=== Initializing Blue Dot ===\n");
    blueDot->active = false;
    blueDot->pulseScale = 1.0f;
    blueDot->pulseDirection = -1.0f;
    blueDot->pulseSpeed = 1.125f;
    blueDot->minScale = 0.7f;
    blueDot->maxScale = 1.0f;
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "assets/dots/blue_dot.bmp");
    blueDot->bitmap = SDL_LoadBMP(filepath);
    if (!blueDot->bitmap) {
        fprintf(debugFile, "Failed to load blue dot bitmap: %s\n", SDL_GetError());
        fclose(debugFile);
        return false;
    
}
    blueDot->texture = SDL_CreateTextureFromSurface(game->renderer, blueDot->bitmap);
    if (!blueDot->texture) {
        fprintf(debugFile, "Failed to create blue dot texture: %s\n", SDL_GetError());
        fclose(debugFile);
        return false;
    
}
    fprintf(debugFile, "Successfully initialized blue dot\n");
    fclose(debugFile);
    return true;
}
void updateBlueDot(BlueDot* blueDot, float deltaTime) {
    if (!blueDot->active) return;
    blueDot->pulseScale += blueDot->pulseDirection * blueDot->pulseSpeed * deltaTime;
    if (blueDot->pulseScale <= blueDot->minScale) {
        blueDot->pulseScale = blueDot->minScale;
        blueDot->pulseDirection = 1.0f;
    
}
    else if (blueDot->pulseScale >= blueDot->maxScale) {
        blueDot->pulseScale = blueDot->maxScale;
        blueDot->pulseDirection = -1.0f;
    
}
}
void placeBlueDot(BlueDot* blueDot, Snake* snake, GameState* game) {
    if (blueDot->active) return;
    bool validPosition = false;
    while (!validPosition) {
        blueDot->x = rand() % (GRID_DIMENSION - 2) + 1;
        blueDot->y = rand() % (GRID_DIMENSION - 2) + 1;
        validPosition = true;
        for (int i = 0; i < snake->length; i++) {
            if (snake->segments[i].x == blueDot->x &&
                snake->segments[i].y == blueDot->y) {
                validPosition = false;
                break;
            
}
        
}
    
}
    blueDot->active = true;
}
bool checkBlueDotCollision(Snake* snake, BlueDot* blueDot) {
    if (!blueDot->active) return false;
    if (snake->segments[0].x == blueDot->x &&
        snake->segments[0].y == blueDot->y) {
        blueDot->active = false;
        return true;
    
}
    return false;
}
void drawBlueDot(BlueDot* blueDot, GameState* game) {
    if (!blueDot->active) return;
    int scaledSize = (int)(game->cellSize * blueDot->pulseScale);
    int offset = (game->cellSize - scaledSize) / 2;
    SDL_Rect dotRect = {
        game->gridOffset + blueDot->x * game->cellSize + offset,
        game->gridOffset + blueDot->y * game->cellSize + offset,
        scaledSize,
        scaledSize
    
};
    SDL_RenderCopy(game->renderer, blueDot->texture, NULL, &dotRect);
}
void cleanupBlueDot(BlueDot* blueDot) {
    if (blueDot->bitmap) {
        SDL_FreeSurface(blueDot->bitmap);
        blueDot->bitmap = NULL;
    
}
    if (blueDot->texture) {
        SDL_DestroyTexture(blueDot->texture);
        blueDot->texture = NULL;
    
}
}
