#pragma once
#include <SDL.h>
struct GameState;
struct Snake;
struct BlueDot {
    int x;
    int y;
    SDL_Surface* bitmap;
    SDL_Texture* texture;
    bool active;
    float pulseScale; 
    float pulseDirection;
    float pulseSpeed;
    float minScale;
    float maxScale;
};
bool initializeBlueDot(BlueDot* blueDot, GameState* game);
void placeBlueDot(BlueDot* blueDot, Snake* snake, GameState* game);
bool checkBlueDotCollision(Snake* snake, BlueDot* blueDot);
void drawBlueDot(BlueDot* blueDot, GameState* game);
void cleanupBlueDot(BlueDot * blueDot);
void updateBlueDot(BlueDot* blueDot, float deltaTime);
