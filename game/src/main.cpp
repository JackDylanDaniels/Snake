#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>
#include "config.h"
#include "game_state.h"
#include "game.h"
#include "renderer.h"
#include "input.h"
int main(int argc, char* argv[]) {
    GameState gameState = { 0 
};
    if (!initializeSDL(&gameState)) {
        return 1;
    
}
    if (!initializeSnake(&gameState.snake, &gameState)) {
        cleanup(&gameState);
        return 1;
    
}
    gameLoop(&gameState);
    cleanup(&gameState);
    return 0;
}
