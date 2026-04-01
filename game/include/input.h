#pragma once
#include "game_state.h"
void handleEvents(GameState* game);
void handleGameOverInput(GameState* game, SDL_Event* event);
void handleMenuInput(GameState* game, SDL_Event* event);
