#pragma once
#include <SDL.h>
struct GameState;
struct GameOver {
    bool active;
    float timer;
    float redTransition;
    bool redTransitionComplete;
    bool showMenu;
    SDL_Surface* gameOverBitmap;
    SDL_Texture* gameOverTexture;
    SDL_Surface* pressNBitmap;
    SDL_Texture* pressNTexture;
    SDL_Surface* pressMBitmap;
    SDL_Texture* pressMTexture;
    SDL_Rect menuRect;
};
bool initializeGameOver(struct GameOver* gameOver, struct GameState* game);
void triggerGameOver(struct GameOver* gameOver, struct GameState* game);
void updateGameOver(struct GameOver* gameOver, struct GameState* game, float deltaTime);
void drawGameOver(struct GameOver* gameOver, struct GameState* game);
void cleanupGameOver(struct GameOver* gameOver);
