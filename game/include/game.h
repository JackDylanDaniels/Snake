#pragma once
#include "game_state.h"
#define SPEEDUP_INTERVAL 10.0f;
#define SPEEDUP_FACTOR 1.2f
bool initializeSDL(GameState* game);
void startGame(GameState* game);
void resetGame(GameState* game);
void gameLoop(GameState* game);
void cleanup(GameState* game);
