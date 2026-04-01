#define _CRT_SECURE_NO_WARNINGS //Dla sprintf -> bez tego musialbym napisac sprintf_s, ale to jedynie dziala na VS2022, nie na innych kompilatorach
#include <stdio.h>
#include <direct.h>
#include <cmath>
#include "snake.h"
#include "game_state.h"
#include "game.h"
int getTurnIndex(Direction from, Direction to) {
    if (from == UP && to == RIGHT) return 0;
    if (from == RIGHT && to == DOWN) return 1;
    if (from == DOWN && to == LEFT) return 2;
    if (from == LEFT && to == UP) return 3;
    if (from == UP && to == LEFT) return 4;
    if (from == RIGHT && to == UP) return 5;
    if (from == DOWN && to == RIGHT) return 6;
    if (from == LEFT && to == DOWN) return 7;
    return 0;
}
bool loadDirectionalBitmaps(SDL_Surface* bitmaps[4], SDL_Texture* textures[4],
    const char* basePath, const char* prefix, SDL_Renderer* renderer, FILE* debugFile) {
    const char* directions[] = { "up", "right", "down", "left" 
};
    for (int dir = 0; dir < 4; dir++) {
        char filepath[256];
        sprintf(filepath, "%s/%s_%s.bmp", basePath, prefix, directions[dir]);
        bitmaps[dir] = SDL_LoadBMP(filepath);
        if (!bitmaps[dir]) {
            fprintf(debugFile, "Failed to load %s: %s\n", filepath, SDL_GetError());
            return false;
        
}
        textures[dir] = SDL_CreateTextureFromSurface(renderer, bitmaps[dir]);
        if (!textures[dir]) {
            fprintf(debugFile, "Failed to create texture for %s: %s\n", filepath, SDL_GetError());
            return false;
        
}
        fprintf(debugFile, "Successfully loaded and created texture for %s\n", filepath);
    
}
    return true;
}
bool loadTurnBitmaps(SDL_Surface* bitmaps[8], SDL_Texture* textures[8],
    const char* basePath, const char* prefix, SDL_Renderer* renderer, FILE* debugFile) {
    const char* turns[] = {
        "up_right", "right_down", "down_left", "left_up",
        "up_left", "right_up", "down_right", "left_down"
    
};
    for (int i = 0; i < 8; i++) {
        char filepath[256];
        sprintf(filepath, "%s/turn/%s_%s.bmp", basePath, prefix, turns[i]);
        bitmaps[i] = SDL_LoadBMP(filepath);
        if (!bitmaps[i]) {
            fprintf(debugFile, "Failed to load %s: %s\n", filepath, SDL_GetError());
            return false;
        
}
        textures[i] = SDL_CreateTextureFromSurface(renderer, bitmaps[i]);
        if (!textures[i]) {
            fprintf(debugFile, "Failed to create texture for %s: %s\n", filepath, SDL_GetError());
            return false;
        
}
        fprintf(debugFile, "Successfully loaded and created texture for %s\n", filepath);
    
}
    return true;
}
void updateSnakeAnimation(Snake* snake, float deltaTime) {
    for (int i = 1; i < snake->length - 1; i++) {
        SnakeSegment* segment = &snake->segments[i];
        segment->pulseScale += segment->pulseDirection * SNAKE_PULSE_SPEED * deltaTime;
        if (segment->pulseScale <= SNAKE_MIN_SCALE) {
            segment->pulseScale = SNAKE_MIN_SCALE;
            segment->pulseDirection = 1.0f;
            segment->isExpanded = false;
        
}
        else if (segment->pulseScale >= SNAKE_MAX_SCALE) {
            segment->pulseScale = SNAKE_MAX_SCALE;
            segment->pulseDirection = -1.0f;
            segment->isExpanded = true;
        
}
    
}
}
bool initializeSnake(Snake* snake, GameState* game) {
    snake->smoothMovement = false;
    FILE* debugFile = fopen("debug.txt", "a");
    fprintf(debugFile, "\n=== Starting Snake Initialization ===\n");
    snake->length = 4;
    snake->direction = RIGHT;
    snake->nextDirection = RIGHT;
    snake->moveProgress = 0.0f;
    snake->currentSpeed = 0.5f;
    snake->canChangeDirection = true;
    snake->baseSpeed = 0.5f;
    snake->currentSpeed = snake->baseSpeed;
    snake->speedMultiplier = 1.0f;
    snake->timeUntilSpeedup = SPEEDUP_INTERVAL;
    snake->segments = (SnakeSegment*)malloc(snake->length * sizeof(SnakeSegment));
    if (!snake->segments) {
        fprintf(debugFile, "Failed to allocate memory for snake segments\n");
        fclose(debugFile);
        return false;
    
}
    int startX = GRID_DIMENSION - 4;
    int startY = GRID_DIMENSION - 4;
    fprintf(debugFile, "Setting initial snake position: x=%d, y=%d\n", startX, startY);
    for (int i = 0; i < snake->length; i++) {
        snake->segments[i].x = startX - i;
        snake->segments[i].y = startY; 
        snake->segments[i].animState = CENTER;
        snake->segments[i].animOffset = 0.0f;
        snake->segments[i].isFirstBodyType = (i % 2 == 0);
        snake->segments[i].type = STRAIGHT;
        snake->segments[i].direction = RIGHT;
        if (i == 0 || i == snake->length - 1) {
            snake->segments[i].pulseScale = 1.0f;
            snake->segments[i].pulseDirection = 0.0f;
        
}
        else {
            snake->segments[i].pulseScale = snake->segments[i].isFirstBodyType ?
                SNAKE_MAX_SCALE : SNAKE_MIN_SCALE;
            snake->segments[i].pulseDirection = snake->segments[i].isFirstBodyType ?
                -1.0f : 1.0f;
            snake->segments[i].isExpanded = snake->segments[i].isFirstBodyType;
        
}
    
}
    for (int dir = 0; dir < 4; dir++) {
        snake->headBitmaps[dir] = NULL;
        snake->body1Bitmaps[dir] = NULL;
        snake->body2Bitmaps[dir] = NULL;
        snake->tailBitmaps[dir] = NULL;
        snake->headTextures[dir] = NULL;
        snake->body1Textures[dir] = NULL;
        snake->body2Textures[dir] = NULL;
        snake->tailTextures[dir] = NULL;
    
}
    for (int i = 0; i < 8; i++) {
        snake->body1TurnBitmaps[i] = NULL;
        snake->body2TurnBitmaps[i] = NULL;
        snake->body1TurnTextures[i] = NULL;
        snake->body2TurnTextures[i] = NULL;
    
}
    if (!loadDirectionalBitmaps(snake->headBitmaps, snake->headTextures,
        "assets/snake/head", "head", game->renderer, debugFile)) {
        fprintf(debugFile, "Failed to load head bitmaps\n");
        fclose(debugFile);
        return false;
    
}
    if (!loadDirectionalBitmaps(snake->body1Bitmaps, snake->body1Textures,
        "assets/snake/body1/straight", "body1", game->renderer, debugFile)) {
        fprintf(debugFile, "Failed to load body1 bitmaps\n");
        fclose(debugFile);
        return false;
    
}
    if (!loadDirectionalBitmaps(snake->body2Bitmaps, snake->body2Textures,
        "assets/snake/body2/straight", "body2", game->renderer, debugFile)) {
        fprintf(debugFile, "Failed to load body2 bitmaps\n");
        fclose(debugFile);
        return false;
    
}
    if (!loadTurnBitmaps(snake->body1TurnBitmaps, snake->body1TurnTextures,
        "assets/snake/body1", "body1", game->renderer, debugFile)) {
        fprintf(debugFile, "Failed to load body1 turn bitmaps\n");
        fclose(debugFile);
        return false;
    
}
    if (!loadTurnBitmaps(snake->body2TurnBitmaps, snake->body2TurnTextures,
        "assets/snake/body2", "body2", game->renderer, debugFile)) {
        fprintf(debugFile, "Failed to load body2 turn bitmaps\n");
        fclose(debugFile);
        return false;
    
}
    if (!loadDirectionalBitmaps(snake->tailBitmaps, snake->tailTextures,
        "assets/snake/tail", "tail", game->renderer, debugFile)) {
        fprintf(debugFile, "Failed to load tail bitmaps\n");
        fclose(debugFile);
        return false;
    
}
    fprintf(debugFile, "Snake initialization completed\n");
    fclose(debugFile);
    return true;
}
bool checkCollision(Snake* snake) {
    int headX = snake->segments[0].x;
    int headY = snake->segments[0].y;
    for (int i = 1; i < snake->length; i++) {
        if (snake->segments[i].x == headX &&
            snake->segments[i].y == headY) {
            return true;
        
}
    
}
    return false;
}
bool canMoveInDirection(Snake* snake, Direction dir) {
    int nextX = snake->segments[0].x;
    int nextY = snake->segments[0].y;
    switch (dir) {
    case UP:    nextY--; break;
    case DOWN:  nextY++; break;
    case LEFT:  nextX--; break;
    case RIGHT: nextX++; break;
    
}
    for (int i = 1; i < snake->length; i++) {
        if (snake->segments[i].x == nextX &&
            snake->segments[i].y == nextY) {
            return false;
        
}
    
}
    return true;
}
Direction getRightTurn(Direction currentDir) {
    switch (currentDir) {
    case UP:    return RIGHT;
    case RIGHT: return DOWN;
    case DOWN:  return LEFT;
    case LEFT:  return UP;
    default:    return RIGHT;
    
}
}
Direction getLeftTurn(Direction currentDir) {
    switch (currentDir) {
    case UP:    return LEFT;
    case RIGHT: return UP;
    case DOWN:  return RIGHT;
    case LEFT:  return DOWN;
    default:    return LEFT;
    
}
}
void handleBorderTurn(Snake* snake) {
    Direction rightTurn = getRightTurn(snake->direction);
    if (canMoveInDirection(snake, rightTurn)) {
        snake->direction = rightTurn;
        snake->nextDirection = rightTurn;
    
}
    else {
        Direction leftTurn = getLeftTurn(snake->direction);
        snake->direction = leftTurn;
        snake->nextDirection = leftTurn;
    
}
}
void moveSnake(Snake* snake) {
    if (snake->moveProgress >= 1.0f) {
        Direction prevDirection = snake->direction;
        int prevX = snake->segments[0].x;
        int prevY = snake->segments[0].y;
        switch (snake->direction) {
        case UP:    snake->segments[0].y--; break;
        case DOWN:  snake->segments[0].y++; break;
        case LEFT:  snake->segments[0].x--; break;
        case RIGHT: snake->segments[0].x++; break;
        
}
        if (snake->segments[0].x < 0 || snake->segments[0].x >= GRID_DIMENSION ||
            snake->segments[0].y < 0 || snake->segments[0].y >= GRID_DIMENSION) {
            Direction rightTurn = snake->direction;
            switch (rightTurn) {
            case UP:    rightTurn = RIGHT; break;
            case RIGHT: rightTurn = DOWN; break;
            case DOWN:  rightTurn = LEFT; break;
            case LEFT:  rightTurn = UP; break;
            
}
            snake->segments[0].x = prevX;
            snake->segments[0].y = prevY;
            snake->direction = rightTurn;
            snake->nextDirection = rightTurn;
            switch (snake->direction) {
            case UP:    snake->segments[0].y--; break;
            case DOWN:  snake->segments[0].y++; break;
            case LEFT:  snake->segments[0].x--; break;
            case RIGHT: snake->segments[0].x++; break;
            
}
        
}
        if (checkCollision(snake)) {
            snake->segments[0].x = prevX;
            snake->segments[0].y = prevY;
            GameState* game = (GameState*)((char*)snake - offsetof(GameState, snake));
            triggerGameOver(&game->gameOver, game);
            return;
        
}
        snake->direction = snake->nextDirection;
        snake->segments[0].direction = snake->direction;
        for (int i = 1; i < snake->length; i++) {
            int tempX = snake->segments[i].x;
            int tempY = snake->segments[i].y;
            Direction tempDir = snake->segments[i].direction;
            snake->segments[i].x = prevX;
            snake->segments[i].y = prevY;
            if (tempDir != prevDirection) {
                snake->segments[i].type = TURNING;
                snake->segments[i].turn.from = tempDir;
                snake->segments[i].turn.to = prevDirection;
            
}
            else {
                snake->segments[i].type = STRAIGHT;
            
}
            snake->segments[i].direction = prevDirection;
            prevX = tempX;
            prevY = tempY;
            prevDirection = tempDir;
        
}
        snake->moveProgress = 0.0f;
    
}
}
void drawSnake(Snake* snake, GameState* game) {
    FILE* debugFile = fopen("debug.txt", "a");
    fprintf(debugFile, "\n=== Drawing Snake ===\n");
    SDL_Rect viewport;
    SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderGetViewport(game->renderer, &viewport);
    float positions[MAX_SNAKE_LENGTH][2];
    if (snake->smoothMovement) {
        positions[0][0] = snake->segments[0].x;
        positions[0][1] = snake->segments[0].y;
        switch (snake->direction) {
        case UP:    positions[0][1] -= snake->moveProgress; break;
        case DOWN:  positions[0][1] += snake->moveProgress; break;
        case LEFT:  positions[0][0] -= snake->moveProgress; break;
        case RIGHT: positions[0][0] += snake->moveProgress; break;
        
}
        for (int i = 1; i < snake->length; i++) {
            float baseX = snake->segments[i].x;
            float baseY = snake->segments[i].y;
            float dx = snake->segments[i - 1].x - baseX;
            float dy = snake->segments[i - 1].y - baseY;
            float segmentProgress;
            if (snake->length <= 20) {
                segmentProgress = snake->moveProgress * (1.0f - (0.05f * i));
            
}
            else {
                float scaleFactor = 0.05f * (20.0f / snake->length);
                segmentProgress = snake->moveProgress * (1.0f - (scaleFactor * i));
            
}
            if (segmentProgress < 0) segmentProgress = 0;
            if (abs(dx) > 0.01f || abs(dy) > 0.01f) {
                positions[i][0] = baseX + dx * segmentProgress;
                positions[i][1] = baseY + dy * segmentProgress;
            
}
            else {
                positions[i][0] = baseX;
                positions[i][1] = baseY;
            
}
            float distToPrev = sqrt(
                (positions[i][0] - positions[i - 1][0]) * (positions[i][0] - positions[i - 1][0]) +
                (positions[i][1] - positions[i - 1][1]) * (positions[i][1] - positions[i - 1][1])
            );
            if (distToPrev > 1.0f) {
                float adjustX = (positions[i - 1][0] - positions[i][0]) / distToPrev;
                float adjustY = (positions[i - 1][1] - positions[i][1]) / distToPrev;
                positions[i][0] = positions[i - 1][0] - adjustX;
                positions[i][1] = positions[i - 1][1] - adjustY;
            
}
        
}
    
}
    else {
        for (int i = 0; i < snake->length; i++) {
            positions[i][0] = snake->segments[i].x;
            positions[i][1] = snake->segments[i].y;
        
}
    
}
    for (int i = 0; i < snake->length; i++) {
        float baseSize = (float)game->cellSize;
        float scaledSize = baseSize;
        float offset = 0;
        if (i > 0 && i < snake->length - 1) {
            scaledSize = baseSize * snake->segments[i].pulseScale;
            offset = (baseSize - scaledSize) / 2.0f;
        
}
        SDL_Rect destRect = {
            game->gridOffset + (int)(positions[i][0] * game->cellSize + offset + 0.5f),
            game->gridOffset + (int)(positions[i][1] * game->cellSize + offset + 0.5f),
            (int)scaledSize,
            (int)scaledSize
        
};
        fprintf(debugFile, "Drawing segment %d at: x=%d, y=%d\n", i, destRect.x, destRect.y);
        SDL_Texture* currentTexture = NULL;
        if (i == 0) {
            fprintf(debugFile, "Drawing head\n");
            currentTexture = snake->headTextures[snake->direction];
        
}
        else if (i == snake->length - 1) {
            fprintf(debugFile, "Drawing tail\n");
            currentTexture = snake->tailTextures[snake->segments[i].direction];
        
}
        else {
            if (snake->segments[i].type == STRAIGHT) {
                fprintf(debugFile, "Drawing straight body segment\n");
                currentTexture = snake->segments[i].isFirstBodyType ?
                    snake->body1Textures[snake->segments[i].direction] :
                    snake->body2Textures[snake->segments[i].direction];
            
}
            else {
                fprintf(debugFile, "Drawing turning body segment\n");
                int turnIndex = getTurnIndex(snake->segments[i].turn.from,
                    snake->segments[i].turn.to);
                currentTexture = snake->segments[i].isFirstBodyType ?
                    snake->body1TurnTextures[turnIndex] :
                    snake->body2TurnTextures[turnIndex];
            
}
        
}
        if (currentTexture) {
            if (game->gameOver.active) {
                SDL_SetTextureColorMod(currentTexture, 255, 0, 0);
                fprintf(debugFile, "Setting texture to red: ");
                if (i == 0) fprintf(debugFile, "HEAD\n");
                else if (i == snake->length - 1) fprintf(debugFile, "TAIL\n");
                else fprintf(debugFile, "BODY%d\n", snake->segments[i].isFirstBodyType ? 1 : 2);
            
}
            SDL_RenderCopy(game->renderer, currentTexture, NULL, &destRect);
        
}
    
}
    fprintf(debugFile, "=== Snake Drawing Complete ===\n\n");
    fclose(debugFile);
}
void growSnake(Snake* snake) {
    if (snake->length >= MAX_SNAKE_LENGTH) return;
    FILE* debugFile = fopen("debug.txt", "a");
    fprintf(debugFile, "\n=== Growing Snake ===\n");
    SnakeSegment* newSegments = (SnakeSegment*)realloc(snake->segments,
        (snake->length + 1) * sizeof(SnakeSegment));
    if (newSegments) {
        snake->segments = newSegments;
        int lastIdx = snake->length - 1;
        snake->segments[snake->length] = snake->segments[lastIdx];
        snake->segments[snake->length].isFirstBodyType = !(snake->segments[lastIdx].isFirstBodyType);
        snake->segments[snake->length].animState = CENTER;
        snake->segments[snake->length].animOffset = 0.0f;
        snake->segments[snake->length].type = STRAIGHT;
        snake->segments[snake->length].direction = snake->segments[lastIdx].direction;
        snake->segments[snake->length].pulseScale =
            snake->segments[snake->length].isFirstBodyType ?
            SNAKE_MAX_SCALE : SNAKE_MIN_SCALE;
        snake->segments[snake->length].pulseDirection =
            snake->segments[snake->length].isFirstBodyType ?
            -1.0f : 1.0f;
        snake->segments[snake->length].isExpanded =
            snake->segments[snake->length].isFirstBodyType;
        snake->length++;
        fprintf(debugFile, "Snake grew to length %d\n", snake->length);
        fprintf(debugFile, "New segment uses %s body type\n",
            snake->segments[snake->length - 1].isFirstBodyType ? "first" : "second");
    
}
    else {
        fprintf(debugFile, "Failed to allocate memory for new segment\n");
    
}
    fclose(debugFile);
}
void cleanupSnake(Snake* snake) {
    if (snake->segments) {
        free(snake->segments);
    
}
    for (int dir = 0; dir < 4; dir++) {
        if (snake->headBitmaps[dir]) SDL_FreeSurface(snake->headBitmaps[dir]);
        if (snake->body1Bitmaps[dir]) SDL_FreeSurface(snake->body1Bitmaps[dir]);
        if (snake->body2Bitmaps[dir]) SDL_FreeSurface(snake->body2Bitmaps[dir]);
        if (snake->tailBitmaps[dir]) SDL_FreeSurface(snake->tailBitmaps[dir]);
        if (snake->headTextures[dir]) SDL_DestroyTexture(snake->headTextures[dir]);
        if (snake->body1Textures[dir]) SDL_DestroyTexture(snake->body1Textures[dir]);
        if (snake->body2Textures[dir]) SDL_DestroyTexture(snake->body2Textures[dir]);
        if (snake->tailTextures[dir]) SDL_DestroyTexture(snake->tailTextures[dir]);
    
}
    for (int i = 0; i < 8; i++) {
        if (snake->body1TurnBitmaps[i]) SDL_FreeSurface(snake->body1TurnBitmaps[i]);
        if (snake->body2TurnBitmaps[i]) SDL_FreeSurface(snake->body2TurnBitmaps[i]);
        if (snake->body1TurnTextures[i]) SDL_DestroyTexture(snake->body1TurnTextures[i]);
        if (snake->body2TurnTextures[i]) SDL_DestroyTexture(snake->body2TurnTextures[i]);
    
}
}
