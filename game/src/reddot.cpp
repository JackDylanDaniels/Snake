#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "redDot.h"
#include "snake.h"
#include "game_state.h"
#include "text.h"
int randomSpawnDelay(void) {
    return 5 + (SDL_GetTicks() % 11);
}
bool initializeRedDot(RedDot* redDot, GameState* game) {
    FILE* debugFile = fopen("debug.txt", "a");
    fprintf(debugFile, "\n=== Initializing Red Dot ===\n");
    redDot->active = false;
    redDot->isPlaced = false;
    redDot->bitmap = SDL_LoadBMP("./assets/dots/red_dot.bmp");
    redDot->wasEatenThisCycle = false;
    redDot->pulseScale = 1.0f;
    redDot->pulseDirection = -1.0f;
    redDot->pulseSpeed = 1.125f;
    redDot->minScale = 0.7f;
    redDot->maxScale = 1.0f;
    if (!redDot->bitmap) {
        fprintf(debugFile, "Failed to load red dot BMP: %s\n", SDL_GetError());
        fclose(debugFile);
        return false;
    
}
    redDot->texture = SDL_CreateTextureFromSurface(game->renderer, redDot->bitmap);
    if (!redDot->texture) {
        fprintf(debugFile, "Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(redDot->bitmap);
        fclose(debugFile);
        return false;
    
}
    fprintf(debugFile, "Red dot initialized successfully with texture\n");
    fclose(debugFile);
    return true;
}
void placeRedDot(RedDot* redDot, Snake* snake, BlueDot* blueDot, GameState* game) {
    if (!redDot->active || redDot->isPlaced) return;
    FILE* debugFile = fopen("debug.txt", "a");
    fprintf(debugFile, "\n=== Attempting to Place Red Dot ===\n");
    bool validPosition = false;
    while (!validPosition) {
        redDot->x = rand() % (GRID_DIMENSION - 2) + 1;
        redDot->y = rand() % (GRID_DIMENSION - 2) + 1;
        validPosition = true;
        for (int i = 0; i < snake->length; i++) {
            if (snake->segments[i].x == redDot->x &&
                snake->segments[i].y == redDot->y) {
                validPosition = false;
                break;
            
}
        
}
        if (validPosition && blueDot->active) {
            if (redDot->x == blueDot->x && redDot->y == blueDot->y) {
                validPosition = false;
            
}
        
}
    
}
    redDot->isPlaced = true;
    fprintf(debugFile, "Successfully placed red dot at (%d, %d)\n", redDot->x, redDot->y);
    fclose(debugFile);
}
bool checkRedDotCollision(Snake* snake, RedDot* redDot) {
    if (!redDot->active || !redDot->isPlaced) return false;
    if (snake->segments[0].x == redDot->x &&
        snake->segments[0].y == redDot->y) {
        redDot->isPlaced = false;
        redDot->active = false;
        redDot->wasEatenThisCycle = true;
        redDot->showCollectedMessage = true;
        redDot->showFailedMessage = false;
        redDot->messageTimer = 3.0f;
        if (rand() % 2 == 0 && snake->length > 3) {
            snake->length--;
            snprintf(redDot->bonusStr, sizeof(redDot->bonusStr), "Collected! Shortening snake by 2");
        
}
        else {
            snake->speedMultiplier *= 0.8f;
            snake->currentSpeed = snake->baseSpeed * snake->speedMultiplier;
            snprintf(redDot->bonusStr, sizeof(redDot->bonusStr), "Collected! Slowing snake by 20%%");
        
}
        return true;
    
}
    return false;
}
void updateRedDot(RedDot* redDot, float deltaTime, GameState* game) {
    FILE* debugFile = fopen("debug.txt", "a");
    fprintf(debugFile, "\n=== Update Red Dot ===\n");
    int currentCycle = (int)(game->gameTime / 10.0f);
    float timeInCycle = game->gameTime - (currentCycle * 10.0f);
    fprintf(debugFile, "Game Time: %.2f, Cycle: %d, Time in cycle: %.2f\n",
        game->gameTime, currentCycle, timeInCycle);
    if (timeInCycle >= 5.0f && timeInCycle < 10.0f) {
        if (!redDot->active && !redDot->wasEatenThisCycle) { 
            redDot->active = true;
            redDot->isPlaced = false;
            fprintf(debugFile, "Activating red dot at game time %.2f\n", game->gameTime);
        
}
    
}
    else {
        if (timeInCycle < 5.0f) {
            redDot->wasEatenThisCycle = false;
        
}
        if (redDot->active) {
            redDot->active = false;
            redDot->isPlaced = false;
            fprintf(debugFile, "Deactivating red dot at game time %.2f\n", game->gameTime);
        
}
    
}
    if (redDot->showCollectedMessage) {
        redDot->messageTimer -= deltaTime;
        if (redDot->messageTimer <= 0.0f) {
            redDot->showCollectedMessage = false;
        
}
    
}
    fclose(debugFile);
}
void updateRedDotPulse(RedDot* redDot, float deltaTime) {
    if (!redDot->active || !redDot->isPlaced) return;
    redDot->pulseScale += redDot->pulseDirection * redDot->pulseSpeed * deltaTime;
    if (redDot->pulseScale <= redDot->minScale) {
        redDot->pulseScale = redDot->minScale;
        redDot->pulseDirection = 1.0f;
    
}
    else if (redDot->pulseScale >= redDot->maxScale) {
        redDot->pulseScale = redDot->maxScale;
        redDot->pulseDirection = -1.0f;
    
}
}
void drawRedDot(RedDot* redDot, GameState* game) {
    if (!redDot->active || !redDot->isPlaced) return;
    int scaledSize = (int)(game->cellSize * redDot->pulseScale);
    int offset = (game->cellSize - scaledSize) / 2;
    SDL_Rect dotRect = {
        game->gridOffset + redDot->x * game->cellSize + offset,
        game->gridOffset + redDot->y * game->cellSize + offset,
        scaledSize,
        scaledSize
    
};
    SDL_RenderCopy(game->renderer, redDot->texture, NULL, &dotRect);
}
void cleanupRedDot(RedDot* redDot) {
    if (redDot->bitmap) {
        SDL_FreeSurface(redDot->bitmap);
        redDot->bitmap = NULL;
    
}
    if (redDot->texture) {
        SDL_DestroyTexture(redDot->texture);
        redDot->texture = NULL;
    
}
}
