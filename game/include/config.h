#pragma once
const int CELL_SIZE = 40;
const int GRID_DIMENSION = 16;
const int BORDER_WIDTH = 4;
const float STATUS_BAR_RATIO = 0.2f;
const float PROGRESS_BAR_RATIO = 0.1f;
enum GameScreen {
    MAIN_MENU,
    PLAY_GAME,
    SETTINGS,
    CHARACTER_SELECT,
    RANKINGS,
    EXIT_GAME
};
enum MenuOption {
    MENU_PLAY,
    MENU_SETTINGS,
    MENU_CHARACTER,
    MENU_RANKINGS,
    MENU_EXIT,
    MENU_OPTION_COUNT
};
