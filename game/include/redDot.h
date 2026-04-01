#pragma once
#include <SDL.h>
struct GameState;
struct Snake;
struct BlueDot;
struct RedDot {
    int x;
    int y;
    SDL_Surface* bitmap;
    SDL_Texture* texture;
    bool active;
    float spawnTimer;
    float activeTimer;
    bool showCollectedMessage;
    bool showFailedMessage;
    float messageTimer;
    char timeRemainingStr[32];
    char bonusStr[32];
    bool isPlaced;
    bool wasEatenThisCycle;
    float pulseScale;
    float pulseDirection;
    float pulseSpeed;
    float minScale;
    float maxScale;
};
bool initializeRedDot(RedDot* redDot, GameState* game);
void placeRedDot(RedDot* redDot, Snake* snake, BlueDot* blueDot, GameState* game);
bool checkRedDotCollision(Snake* snake, RedDot* redDot);
void drawRedDot(RedDot* redDot, GameState* game);
void cleanupRedDot(RedDot* redDot);
void updateRedDot(RedDot* redDot, float deltaTime, GameState* game);
void updateRedDotPulse(RedDot* redDot, float deltaTime);
