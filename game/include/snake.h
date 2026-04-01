#pragma once
#include <SDL.h>
#define MAX_SNAKE_LENGTH 52*52
#define SNAKE_PULSE_SPEED 1.125f
#define SNAKE_MAX_SCALE 1.0f
#define SNAKE_MIN_SCALE 0.85f //1.0f Brak Animacji
struct GameState;
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
enum AnimationState {
    CENTER,
    LEFT_SHIFT,
    RIGHT_SHIFT
};
enum SegmentType {
    STRAIGHT,
    TURNING
};
struct Turn {
    Direction from;
    Direction to;
};
struct SnakeSegment {
    int x;
    int y;
    AnimationState animState;
    float animOffset;
    bool isFirstBodyType;
    SegmentType type;
    Turn turn;
    Direction direction;
    float pulseScale;
    float pulseDirection;
    bool isExpanded;
};
struct Snake {
    SnakeSegment* segments;
    int length;
    Direction direction;
    Direction nextDirection;
    SDL_Surface* headBitmaps[4];
    SDL_Texture* headTextures[4];
    SDL_Surface* body1Bitmaps[4];
    SDL_Surface* body2Bitmaps[4];
    SDL_Texture* body1Textures[4];
    SDL_Texture* body2Textures[4];
    SDL_Surface* body1TurnBitmaps[8];
    SDL_Surface* body2TurnBitmaps[8];
    SDL_Texture* body1TurnTextures[8];
    SDL_Texture* body2TurnTextures[8];
    SDL_Surface* tailBitmaps[4];
    SDL_Texture* tailTextures[4];
    float moveProgress;
    float currentSpeed;
    bool canChangeDirection;
    bool smoothMovement;
    bool isInvulnerable;
    float invulnerabilityTimer;
    const float INVULNERABILITY_DURATION = 3.0f;
    float baseSpeed;
    float speedMultiplier;
    float timeUntilSpeedup;
};
bool initializeSnake(Snake* snake, GameState* game);
bool checkCollision(Snake* snake);
void moveSnake(Snake* snake);
void drawSnake(Snake* snake, GameState* game);
void cleanupSnake(Snake* snake);
void growSnake(Snake* snake);
void updateSnakeAnimation(Snake* snake, float deltaTime);
