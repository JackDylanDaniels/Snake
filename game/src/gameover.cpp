#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "gameOver.h"
#include "game_state.h"
bool initializeGameOver(GameOver* gameOver, GameState* game) {
    FILE* debugFile = fopen("debug.txt", "a");
    fprintf(debugFile, "\n=== Initializing Game Over System ===\n");
    gameOver->active = false;
    gameOver->timer = 0.0f;
    gameOver->redTransition = 0.0f;
    gameOver->redTransitionComplete = false;
    gameOver->showMenu = false;
    gameOver->gameOverBitmap = SDL_LoadBMP("assets/menu/game_over.bmp");
    if (!gameOver->gameOverBitmap) {
        fprintf(debugFile, "Failed to load game over bitmap\n");
        fclose(debugFile);
        return false;
    
}
    gameOver->pressNBitmap = SDL_LoadBMP("assets/menu/press_n.bmp");
    if (!gameOver->pressNBitmap) {
        fprintf(debugFile, "Failed to load press N bitmap\n");
        fclose(debugFile);
        return false;
    
}
    gameOver->pressMBitmap = SDL_LoadBMP("assets/menu/press_m.bmp");
    if (!gameOver->pressMBitmap) {
        fprintf(debugFile, "Failed to load press M bitmap\n");
        fclose(debugFile);
        return false;
    
}
    gameOver->gameOverTexture = SDL_CreateTextureFromSurface(game->renderer, gameOver->gameOverBitmap);
    gameOver->pressNTexture = SDL_CreateTextureFromSurface(game->renderer, gameOver->pressNBitmap);
    gameOver->pressMTexture = SDL_CreateTextureFromSurface(game->renderer, gameOver->pressMBitmap);
    if (!gameOver->gameOverTexture || !gameOver->pressNTexture || !gameOver->pressMTexture) {
        fprintf(debugFile, "Failed to create textures\n");
        fclose(debugFile);
        return false;
    
}
    fprintf(debugFile, "Game over system initialized successfully\n");
    fclose(debugFile);
    return true;
}
void triggerGameOver(GameOver* gameOver, GameState* game) {
    FILE* debugFile = fopen("debug.txt", "a");
    fprintf(debugFile, "\n=== Game Over Triggered ===\n");
    gameOver->active = true;
    gameOver->timer = 0.0f;
    gameOver->redTransition = 0.0f;
    gameOver->redTransitionComplete = false;
    gameOver->showMenu = true;
    fprintf(debugFile, "Game over state activated\n");
    fclose(debugFile);
}
void updateGameOver(GameOver* gameOver, GameState* game, float deltaTime) {
    if (!gameOver->active) return;
    FILE* debugFile = fopen("debug.txt", "a");
    fprintf(debugFile, "\n=== Game Over Update ===\n");
    fprintf(debugFile, "Active: %s, ShowMenu: %s\n",
        gameOver->active ? "true" : "false",
        gameOver->showMenu ? "true" : "false");
    fclose(debugFile);
    gameOver->timer += deltaTime;
    if (!gameOver->redTransitionComplete) {
        gameOver->redTransition = gameOver->timer;
        if (gameOver->timer >= 1.0f) {
            gameOver->redTransitionComplete = true;
        
}
    
}
    if (gameOver->timer >= 2.0f && !gameOver->showMenu) {
        gameOver->showMenu = true;
    
}
    fprintf(debugFile, "Final state:\n");
    fprintf(debugFile, "- Timer: %.2f\n", gameOver->timer);
    fprintf(debugFile, "- Red transition complete: %s\n", gameOver->redTransitionComplete ? "true" : "false");
    fprintf(debugFile, "- Show menu: %s\n", gameOver->showMenu ? "true" : "false");
    fclose(debugFile);
}
void drawGameOver(GameOver* gameOver, GameState* game) {
    if (!gameOver->active) return;
    SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 192);
    SDL_Rect overlay = { 0, 0, game->screenWidth, game->screenHeight 
};
    SDL_RenderFillRect(game->renderer, &overlay);
    int menuWidth = 600;
    int menuHeight = 300;
    SDL_Rect menuRect = {
        (game->screenWidth - menuWidth) / 2,
        (game->screenHeight - menuHeight) / 2,
        menuWidth,
        menuHeight
    
};
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(game->renderer, &menuRect);
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    for (int i = 0; i < 4; i++) {
        SDL_Rect border = {
            menuRect.x - i,
            menuRect.y - i,
            menuRect.w + 2 * i,
            menuRect.h + 2 * i
        
};
        SDL_RenderDrawRect(game->renderer, &border);
    
}
    if (gameOver->gameOverTexture) {
        int textW, textH;
        SDL_QueryTexture(gameOver->gameOverTexture, NULL, NULL, &textW, &textH);
        SDL_Rect gameOverRect = {
            menuRect.x + (menuWidth - textW) / 2,
            menuRect.y + 50,
            textW,
            textH
        
};
        SDL_RenderCopy(game->renderer, gameOver->gameOverTexture, NULL, &gameOverRect);
    
}
    if (gameOver->pressNTexture) {
        int textW, textH;
        SDL_QueryTexture(gameOver->pressNTexture, NULL, NULL, &textW, &textH);
        float scale = 0.4f;
        int scaledWidth = (int)(textW * scale);
        int scaledHeight = (int)(textH * scale);
        SDL_Rect pressNRect = {
            menuRect.x + (menuWidth - scaledWidth) / 2,
            menuRect.y + menuHeight - scaledHeight * 3,
            scaledWidth,
            scaledHeight
        
};
        SDL_RenderCopy(game->renderer, gameOver->pressNTexture, NULL, &pressNRect);
    
}
    if (gameOver->pressMTexture) {
        int textW, textH;
        SDL_QueryTexture(gameOver->pressMTexture, NULL, NULL, &textW, &textH);
        float scale = 0.4f;
        int scaledWidth = (int)(textW * scale);
        int scaledHeight = (int)(textH * scale);
        SDL_Rect pressMRect = {
        menuRect.x + (menuWidth - scaledWidth) / 2,
        menuRect.y + menuHeight - scaledHeight - 20,
        scaledWidth,
        scaledHeight
        
};
        SDL_RenderCopy(game->renderer, gameOver->pressMTexture, NULL, &pressMRect);
    
}
    SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_NONE);
}
void cleanupGameOver(GameOver* gameOver) {
    if (gameOver->gameOverBitmap) SDL_FreeSurface(gameOver->gameOverBitmap);
    if (gameOver->pressNBitmap) SDL_FreeSurface(gameOver->pressNBitmap);
    if (gameOver->pressMBitmap) SDL_FreeSurface(gameOver->pressMBitmap);
    if (gameOver->gameOverTexture) SDL_DestroyTexture(gameOver->gameOverTexture);
    if (gameOver->pressNTexture) SDL_DestroyTexture(gameOver->pressNTexture);
    if (gameOver->pressMTexture) SDL_DestroyTexture(gameOver->pressMTexture);
}
