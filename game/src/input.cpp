#include "input.h"
#include "game.h"
void handleMenuInput(GameState* game, SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
        case SDLK_UP: case 'w': case 'W':
            game->selectedOption = (MenuOption)(
                (game->selectedOption - 1 + MENU_OPTION_COUNT) % MENU_OPTION_COUNT
                );
            break;
        case SDLK_DOWN: case 's': case 'S':
            game->selectedOption = (MenuOption)(
                (game->selectedOption + 1) % MENU_OPTION_COUNT
                );
            break;
        case SDLK_RETURN: case SDLK_SPACE:
            switch (game->selectedOption) {
            case MENU_PLAY:
                game->currentScreen = PLAY_GAME;
                game->score = 0;
                game->gameTime = 0.0f;
                break;
            case MENU_SETTINGS:
                game->currentScreen = SETTINGS;
                break;
            case MENU_CHARACTER:
                game->currentScreen = CHARACTER_SELECT;
                break;
            case MENU_RANKINGS:
                game->currentScreen = RANKINGS;
                break;
            case MENU_EXIT:
                game->isRunning = false;
                break;
            
}
            break;
        
}
    
}
}
void handleGameInput(GameState* game, SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
        case SDLK_UP:
        case SDLK_w:
            if (game->snake.direction != DOWN)
                game->snake.nextDirection = UP;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            if (game->snake.direction != UP)
                game->snake.nextDirection = DOWN;
            break;
        case SDLK_LEFT:
        case SDLK_a:
            if (game->snake.direction != RIGHT)
                game->snake.nextDirection = LEFT;
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            if (game->snake.direction != LEFT)
                game->snake.nextDirection = RIGHT;
            break;
        case SDLK_e:
            game->snake.smoothMovement = !game->snake.smoothMovement;
            break;
        case SDLK_q:
            triggerGameOver(&game->gameOver, game);
            break;
        case SDLK_n:
            game->gameOver.active = false;
            game->gameOver.timer = 0.0f;
            game->gameOver.redTransition = 0.0f;
            game->score = 0;
            game->gameTime = 0.0f;
            game->lastStartTime = SDL_GetTicks() / 1000.0f;
            game->snake.speedMultiplier = 1.0f;
            initializeSnake(&game->snake, game);
            placeBlueDot(&game->blueDot, &game->snake, game);
            break;
        
}
    
}
}
void handleGameOverInput(GameState* game, SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
        case SDLK_m:
            game->gameOver.active = false;
            game->gameOver.timer = 0.0f;
            game->gameOver.redTransition = 0.0f;
            game->score = 0;
            game->totalGameTime = 0;
            game->lastTimeUpdate = SDL_GetTicks();
            game->gameTime = 0.0f;
            game->snake.speedMultiplier = 1.0f;
            initializeSnake(&game->snake, game);
            placeBlueDot(&game->blueDot, &game->snake, game);
            game->currentScreen = MAIN_MENU;
            break;
        case SDLK_n:
            game->gameOver.active = false;
            game->gameOver.timer = 0.0f;
            game->gameOver.redTransition = 0.0f;
            game->score = 0;
            game->totalGameTime = 0;
            game->lastTimeUpdate = SDL_GetTicks();
            game->gameTime = 0.0f;
            game->snake.speedMultiplier = 1.0f;
            initializeSnake(&game->snake, game);
            placeBlueDot(&game->blueDot, &game->snake, game);
            break;
        
}
    
}
}
void handleEvents(GameState* game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            game->isRunning = false;
            return;
        
}
        switch (game->currentScreen) {
        case MAIN_MENU:
            handleMenuInput(game, &event);
            break;
        case PLAY_GAME:
            if (game->gameOver.active) {
                handleGameOverInput(game, &event);
            
}
            else {
                handleGameInput(game, &event);
            
}
            break;
        
}
    
}
}
