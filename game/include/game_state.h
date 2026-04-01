#pragma once
#include <SDL.h>
#include "config.h"
#include "snake.h"
#include "blueDot.h"
#include "redDot.h"
#include "gameOver.h"
struct GameState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;
    int gridOffset;
    bool isRunning;
    GameScreen currentScreen;
    MenuOption selectedOption;
    struct Snake snake;
    struct BlueDot blueDot;
    struct RedDot redDot;
    struct GameOver gameOver;
    int cellSize;
    SDL_Surface* screen;
    SDL_Surface* charset;
    SDL_Texture* scrtex;
    int score;
    float gameTime;
    float lastStartTime;
    char timeStr[32];
    char scoreStr[32];
    char modeStr[32];
    Uint32 totalGameTime;
    Uint32 lastTimeUpdate;
};
