#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "renderer.h"
#include "text.h"
#include "substitutes.h"
void drawGrid(GameState* game) {
    int gridSize = GRID_DIMENSION * game->cellSize;
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_Rect border = {
        game->gridOffset - BORDER_WIDTH,
        game->gridOffset - BORDER_WIDTH,
        gridSize + 2 * BORDER_WIDTH,
        gridSize + 2 * BORDER_WIDTH
    
};
    SDL_RenderFillRect(game->renderer, &border);
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_Rect gridBackground = {
        game->gridOffset,
        game->gridOffset,
        gridSize,
        gridSize
    
};
    SDL_RenderFillRect(game->renderer, &gridBackground);
    SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 255);
    for (int i = 0; i <= GRID_DIMENSION; i++) {
        SDL_RenderDrawLine(game->renderer,
            game->gridOffset + i * game->cellSize, game->gridOffset,
            game->gridOffset + i * game->cellSize, game->gridOffset + gridSize);
        SDL_RenderDrawLine(game->renderer,
            game->gridOffset, game->gridOffset + i * game->cellSize,
            game->gridOffset + gridSize, game->gridOffset + i * game->cellSize);
    
}
}
void drawStatusBar(GameState* game) {
    int gridSize = GRID_DIMENSION * game->cellSize;
    int gridRightEdge = game->gridOffset + gridSize + BORDER_WIDTH;
    int spaceAfterGrid = game->screenWidth - gridRightEdge;
    int progressBarWidth = (int)(spaceAfterGrid * PROGRESS_BAR_RATIO);
    int statusBarWidth = (int)(spaceAfterGrid * (1.0f - PROGRESS_BAR_RATIO));
    int statusBarHeight = gridSize;
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_Rect progressBarBorder = {
        gridRightEdge + BORDER_WIDTH,
        game->gridOffset - BORDER_WIDTH,
        progressBarWidth,
        statusBarHeight + 2 * BORDER_WIDTH
    
};
    SDL_RenderFillRect(game->renderer, &progressBarBorder);
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_Rect progressBarBackground = {
        gridRightEdge + 2 * BORDER_WIDTH,
        game->gridOffset,
        progressBarWidth - 2 * BORDER_WIDTH,
        statusBarHeight
    
};
    SDL_RenderFillRect(game->renderer, &progressBarBackground);
    if (game->redDot.active) {
        float timeInCycle = game->gameTime - ((int)(game->gameTime / 10.0f) * 10.0f);
        float timeInActiveWindow = timeInCycle - 5.0f;
        float progress = timeInActiveWindow / 5.0f;
        SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
        SDL_Rect timerBar = {
            gridRightEdge + 2 * BORDER_WIDTH,
            game->gridOffset + (int)((1.0f - progress) * statusBarHeight),
            progressBarWidth - 2 * BORDER_WIDTH,
            (int)(progress * statusBarHeight)
        
};
        SDL_RenderFillRect(game->renderer, &timerBar);
    
}
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_Rect statusBarBorder = {
        gridRightEdge + progressBarWidth + 2 * BORDER_WIDTH,
        game->gridOffset - BORDER_WIDTH,
        statusBarWidth - 3 * BORDER_WIDTH,
        statusBarHeight + 2 * BORDER_WIDTH
    
};
    SDL_RenderFillRect(game->renderer, &statusBarBorder);
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_Rect statusBarBackground = {
        gridRightEdge + progressBarWidth + 3 * BORDER_WIDTH,
        game->gridOffset,
        statusBarWidth - 5 * BORDER_WIDTH,
        statusBarHeight
    
};
    SDL_RenderFillRect(game->renderer, &statusBarBackground);
    SDL_Surface* snakeLogo = SDL_LoadBMP("assets/menu/snake.bmp");
    if (snakeLogo != NULL) {
        int titleWidth = (statusBarWidth - 7 * BORDER_WIDTH) / 2;
        int titleHeight = statusBarHeight / 6;
        int titleX = gridRightEdge + progressBarWidth + 3 * BORDER_WIDTH +
            (statusBarWidth - 5 * BORDER_WIDTH - titleWidth) / 2;
        int titleY = game->gridOffset + 10;
        SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
        SDL_Rect titleBorder = {
            titleX - 2,
            titleY - 2,
            titleWidth + 4,
            titleHeight + 4
        
};
        SDL_RenderFillRect(game->renderer, &titleBorder);
        SDL_Rect titleArea = {
            titleX,
            titleY,
            titleWidth,
            titleHeight
        
};
        SDL_Texture* logoTexture = SDL_CreateTextureFromSurface(game->renderer, snakeLogo);
        SDL_RenderCopy(game->renderer, logoTexture, NULL, &titleArea);
        SDL_DestroyTexture(logoTexture);
        SDL_FreeSurface(snakeLogo);
    
}
    SDL_RenderReadPixels(game->renderer, NULL,
        SDL_PIXELFORMAT_ARGB8888,
        game->screen->pixels,
        game->screen->pitch);
    char timeStr[32];
    char scoreStr[32];
    char modeStr[32];
    char speedStr[32];
    sprintf(timeStr, "Time: %02d:%02d", (int)game->gameTime / 60, (int)game->gameTime % 60);
    sprintf(scoreStr, "Score: %d", game->score);
    sprintf(modeStr, "Mode: %s", game->snake.smoothMovement ? "Smooth" : "Grid");
    sprintf(speedStr, "Speed: %.1fx", game->snake.speedMultiplier);
    int statusStartY = game->gridOffset + statusBarHeight / 4 + 30;
    int statusEndY = game->gridOffset + statusBarHeight - 20;
    int availableHeight = statusEndY - statusStartY;
    int numItems = 4;
    int spacing = availableHeight / (numItems - 1);
    int textStartX = gridRightEdge + progressBarWidth + 4 * BORDER_WIDTH;
    DrawString(game->screen, textStartX, statusStartY, timeStr, game->charset);
    DrawString(game->screen, textStartX + 1, statusStartY, timeStr, game->charset);
    DrawString(game->screen, textStartX, statusStartY + spacing, scoreStr, game->charset);
    DrawString(game->screen, textStartX + 1, statusStartY + spacing, scoreStr, game->charset);
    DrawString(game->screen, textStartX, statusStartY + 2 * spacing, modeStr, game->charset);
    DrawString(game->screen, textStartX + 1, statusStartY + 2 * spacing, modeStr, game->charset);
    DrawString(game->screen, textStartX, statusStartY + 3 * spacing, speedStr, game->charset);
    DrawString(game->screen, textStartX + 1, statusStartY + 3 * spacing, speedStr, game->charset);
    if (game->redDot.showCollectedMessage && game->redDot.messageTimer > 0.0f) {
        int messageX = gridRightEdge + progressBarWidth + 3 * BORDER_WIDTH + ((statusBarWidth - 5 * BORDER_WIDTH) / 2) - (strlen(game->redDot.bonusStr) * 8 / 2);
        int messageY = game->gridOffset + (statusBarHeight / 2);
        DrawString(game->screen, messageX, messageY, game->redDot.bonusStr, game->charset);
        DrawString(game->screen, messageX + 1, messageY, game->redDot.bonusStr, game->charset);
    
}
    SDL_UpdateTexture(game->scrtex, NULL, game->screen->pixels, game->screen->pitch);
    SDL_RenderCopy(game->renderer, game->scrtex, NULL, NULL);
}
void drawRedDotStatus(RedDot* redDot, GameState* game) {
    int textStartX = game->gridOffset + (GRID_DIMENSION * game->cellSize) + 150;
    int textY = game->screenHeight / 2;
    if (redDot->active) {
        char activeText[64];
        sprintf(activeText, "Red dot active!");
        DrawString(game->screen, textStartX, textY, activeText, game->charset);
        char timeText[64];
        sprintf(timeText, "Time remaining: %s", redDot->timeRemainingStr);
        DrawString(game->screen, textStartX, textY + 20, timeText, game->charset);
    
}
    else if (redDot->showCollectedMessage && redDot->messageTimer > 0) {
        char collectedText[64];
        sprintf(collectedText, "Collected!");
        DrawString(game->screen, textStartX, textY, collectedText, game->charset);
        char bonusText[64];
        sprintf(bonusText, "Your bonus: %s", redDot->bonusStr);
        DrawString(game->screen, textStartX, textY + 20, bonusText, game->charset);
    
}
    else if (redDot->showFailedMessage && redDot->messageTimer > 0) {
        char failedText[64];
        sprintf(failedText, "Failed to collect");
        DrawString(game->screen, textStartX, textY, failedText, game->charset);
    
}
}
void drawMainMenu(GameState* game) {
    SDL_SetRenderDrawColor(game->renderer, 6, 64, 43, 255);
    SDL_RenderClear(game->renderer);
    SDL_FillRect(game->screen, NULL, SDL_MapRGB(game->screen->format, 6, 64, 43));
    SDL_Surface* snakeLogo = SDL_LoadBMP("assets/menu/snake.bmp");
    if (snakeLogo != NULL) {
        SDL_Rect titleArea = {
            game->screenWidth / 4,
            game->screenHeight / 6,
            game->screenWidth / 2,
            game->screenHeight / 6
        
};
        SDL_Texture* logoTexture = SDL_CreateTextureFromSurface(game->renderer, snakeLogo);
        SDL_RenderCopy(game->renderer, logoTexture, NULL, &titleArea);
        SDL_DestroyTexture(logoTexture);
        SDL_FreeSurface(snakeLogo);
    
}
    const int optionHeight = 50;
    const int optionWidth = game->screenWidth / 3;
    const int startY = game->screenHeight / 2;
    const int spacing = 20;
    for (int i = 0; i < MENU_OPTION_COUNT; i++) {
        SDL_Rect optionRect = {
            (game->screenWidth - optionWidth) / 2,
            startY + i * (optionHeight + spacing),
            optionWidth,
            optionHeight
        
};
        SDL_SetRenderDrawColor(game->renderer,
            i == game->selectedOption ? 192 : 0,
            i == game->selectedOption ? 192 : 0,
            i == game->selectedOption ? 192 : 0,
            255);
        SDL_RenderFillRect(game->renderer, &optionRect);
    
}
    SDL_RenderReadPixels(game->renderer, NULL,
        SDL_PIXELFORMAT_ARGB8888,
        game->screen->pixels,
        game->screen->pitch);
    const char* menuOptions[] = {
        "Play Game",
        "Settings",
        "Character Select",
        "Rankings",
        "Exit Game"
    
};
    const int charWidth = 12;
    const int charHeight = 12;
    for (int i = 0; i < MENU_OPTION_COUNT; i++) {
        int textX = (game->screenWidth - optionWidth) / 2 + (optionWidth - strlen(menuOptions[i]) * charWidth) / 2;
        int textY = startY + i * (optionHeight + spacing) + (optionHeight - charHeight) / 2;
        DrawString(game->screen, textX, textY, menuOptions[i], game->charset);
        DrawString(game->screen, textX + 1, textY, menuOptions[i], game->charset);
    
}
    SDL_UpdateTexture(game->scrtex, NULL, game->screen->pixels, game->screen->pitch);
    SDL_RenderCopy(game->renderer, game->scrtex, NULL, NULL);
}
void render(GameState* game) {
    FILE* debugFile = fopen("debug.txt", "a");
    fprintf(debugFile, "\n=== Render Start ===\n");
    fprintf(debugFile, "Current Screen: %d\n", game->currentScreen);
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);
    SDL_FillRect(game->screen, NULL, 0);
    switch (game->currentScreen) {
    case MAIN_MENU:
        fprintf(debugFile, "Drawing Main Menu\n");
        drawMainMenu(game);
        break;
    case PLAY_GAME:
        fprintf(debugFile, "Drawing Game Screen\n");
        drawGrid(game);
        drawBlueDot(&game->blueDot, game);
        drawRedDot(&game->redDot, game);
        drawSnake(&game->snake, game);
        drawStatusBar(game);
        drawRedDotStatus(&game->redDot, game);
        if (game->gameOver.active) {
            fprintf(debugFile, "Drawing Game Over screen\n");
            drawGameOver(&game->gameOver, game);
        
}
        break;
    default:
        fprintf(debugFile, "Unknown screen state: %d\n", game->currentScreen);
        break;
    
}
    SDL_RenderPresent(game->renderer);
    fprintf(debugFile, "=== Render Complete ===\n");
    fclose(debugFile);
}
