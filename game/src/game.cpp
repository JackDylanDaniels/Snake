#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "snake.h"
#include "gameOver.h"
#include "substitutes.h"
#include "blueDot.h"
#include "redDot.h"
bool initializeSDL(GameState* game) {
    FILE* debugFile = fopen("debug.txt", "w");
    if (debugFile) {
        fprintf(debugFile, "Starting initialization...\n");
        fclose(debugFile);
    
}
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    
}
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    game->screenWidth = displayMode.w;
    game->screenHeight = displayMode.h;
    int maxPossibleCellSize = min(
        (game->screenHeight - 2 * BORDER_WIDTH) / GRID_DIMENSION,
        (game->screenWidth * 0.8 - 2 * BORDER_WIDTH) / GRID_DIMENSION
    );
    game->cellSize = maxPossibleCellSize;
    game->gridOffset = (game->screenHeight - (GRID_DIMENSION * game->cellSize)) / 2;
    game->window = SDL_CreateWindow("ProjektPPnr2 Snake",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        game->screenWidth, game->screenHeight,
        SDL_WINDOW_FULLSCREEN);
    if (!game->window) {
        return false;
    
}
    game->renderer = SDL_CreateRenderer(game->window, -1,
        SDL_RENDERER_ACCELERATED);
    if (!game->renderer) {
        SDL_DestroyWindow(game->window);
        return false;
    
}
    game->screen = SDL_CreateRGBSurface(0, game->screenWidth, game->screenHeight, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!game->screen) {
        return false;
    
}
    game->charset = SDL_LoadBMP("./cs8x8.bmp");
    if (!game->charset) {
        printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
        return false;
    
}
    SDL_SetColorKey(game->charset, true, 0x000000);
    game->scrtex = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        game->screenWidth, game->screenHeight);
    if (!game->scrtex) {
        return false;
    
}
    int gridSize = GRID_DIMENSION * game->cellSize;
    game->gridOffset = (game->screenHeight - gridSize) / 2;
    game->isRunning = true;
    if (!initializeBlueDot(&game->blueDot, game)) {
        return false;
    
}
    if (!initializeRedDot(&game->redDot, game)) {
        return false;
    
}
    if (!initializeGameOver(&game->gameOver, game)) {
        return false;
    
}
    game->score = 0;
    game->gameTime = 0.0f;
    sprintf(game->timeStr, "Time: 00:00");
    sprintf(game->scoreStr, "Score: 0");
    sprintf(game->modeStr, "Mode: Grid");
    game->currentScreen = MAIN_MENU;
    game->selectedOption = MENU_PLAY;
    return true;
}
void startGame(GameState* game) {
    game->currentScreen = PLAY_GAME;
    game->score = 0;
    game->gameTime = 0.0f;
}
void resetGame(GameState* game) {
    game->score = 0;
    game->gameTime = 0.0f;
    game->gameOver.active = false;
    game->gameOver.timer = 0.0f;
    game->gameOver.redTransition = 0.0f;
    game->snake.speedMultiplier = 1.0f;
    game->snake.currentSpeed = game->snake.baseSpeed;
    game->snake.timeUntilSpeedup = SPEEDUP_INTERVAL;
    initializeSnake(&game->snake, game);
    placeBlueDot(&game->blueDot, &game->snake, game);
    game->currentScreen = PLAY_GAME;
}
void gameLoop(GameState* game) {
    Uint32 lastMoveTime = SDL_GetTicks();
    Uint32 lastFrameTime = lastMoveTime;
    const int BASE_MOVE_DELAY = 150;
    int currentMoveDelay = BASE_MOVE_DELAY;
    game->totalGameTime = 0;
    game->lastTimeUpdate = SDL_GetTicks();
    placeBlueDot(&game->blueDot, &game->snake, game);
    float lastSpeedupTime = 0.0f;
    game->redDot.isPlaced = false;
    game->blueDot.pulseScale = 1.0f;
    game->blueDot.pulseDirection = -1.0f;
    game->redDot.pulseScale = 1.0f;
    game->redDot.pulseDirection = -1.0f;
    while (game->isRunning) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (float)(currentTime - lastFrameTime) / 1000.0f;
        FILE* debugFile = fopen("debug.txt", "a");
        fprintf(debugFile, "\n=== Game Loop Timing ===\n");
        fprintf(debugFile, "Current Time: %u\n", currentTime);
        fprintf(debugFile, "Last Frame Time: %u\n", lastFrameTime);
        fprintf(debugFile, "Calculated Delta Time: %f\n", deltaTime);
        fclose(debugFile);
        handleEvents(game);
        if (game->gameTime == 0.0f) {
            currentMoveDelay = BASE_MOVE_DELAY;
            lastSpeedupTime = 0.0f;
        
}
        if (!game->gameOver.active && game->currentScreen == PLAY_GAME) {
            updateBlueDot(&game->blueDot, deltaTime);
            updateRedDotPulse(&game->redDot, deltaTime);
            updateSnakeAnimation(&game->snake, deltaTime);
            game->totalGameTime += currentTime - game->lastTimeUpdate;
            game->gameTime = game->totalGameTime / 1000.0f;
            if (game->gameTime - lastSpeedupTime >= 10.0f) {
                game->snake.speedMultiplier *= 1.2f;
                currentMoveDelay = (int)(BASE_MOVE_DELAY / game->snake.speedMultiplier);
                lastSpeedupTime = game->gameTime;
            
}
            game->snake.moveProgress = (float)(currentTime - lastMoveTime) / currentMoveDelay;
            if (game->snake.moveProgress > 1.0f) {
                game->snake.moveProgress = 1.0f;
            
}
            if (currentTime - lastMoveTime >= currentMoveDelay) {
                moveSnake(&game->snake);
                if (checkBlueDotCollision(&game->snake, &game->blueDot)) {
                    growSnake(&game->snake);
                    placeBlueDot(&game->blueDot, &game->snake, game);
                    game->score++;
                
}
                lastMoveTime = currentTime;
            
}
            updateRedDot(&game->redDot, deltaTime, game);
            if (game->redDot.active && !game->redDot.isPlaced) {
                placeRedDot(&game->redDot, &game->snake, &game->blueDot, game);
                game->redDot.isPlaced = true;
            
}
            if (checkRedDotCollision(&game->snake, &game->redDot)) {
                game->score += 2;
                game->redDot.isPlaced = false;
            
}
        
}
        else {
            updateGameOver(&game->gameOver, game, deltaTime);
        
}
        game->lastTimeUpdate = currentTime;
        render(game);
        lastFrameTime = currentTime;
        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < 16) {
            SDL_Delay(16 - frameTime);
        
}
    
}
}
void cleanup(GameState* game) {
    cleanupGameOver(&game->gameOver);
    cleanupBlueDot(&game->blueDot);
    cleanupRedDot(&game->redDot);
    cleanupSnake(&game->snake);
    SDL_FreeSurface(game->charset);
    SDL_FreeSurface(game->screen);
    SDL_DestroyTexture(game->scrtex);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}
