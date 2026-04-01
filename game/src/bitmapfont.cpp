#define _CRT_SECURE_NO_WARNINGS  
#include "bitmapFont.h"
#include <stdio.h>
bool loadCharacterBitmaps(characterBitmaps* chars, SDL_Renderer* renderer) {
    char filename[100];
    for (int i = 0; i < 128; i++) {
        chars->surfaces[i] = NULL;
        chars->textures[i] = NULL;
    
}
    for (char c = 'A'; c <= 'Z'; c++) {
        snprintf(filename, sizeof(filename), "assets/font/bmp/%c.bmp", c);
        chars->surfaces[c] = SDL_LoadBMP(filename);
        if (!chars->surfaces[c]) {
            printf("Failed to load %c.bmp: %s\n", c, SDL_GetError());
            return false;
        
}
        SDL_SetColorKey(chars->surfaces[c], SDL_TRUE,
            SDL_MapRGB(chars->surfaces[c]->format, 0, 0, 0));
        chars->textures[c] = SDL_CreateTextureFromSurface(renderer, chars->surfaces[c]);
        SDL_FreeSurface(chars->surfaces[c]);
    
}
    for (char c = '0'; c <= '9'; c++) {
        snprintf(filename, sizeof(filename), "assets/font/bmp/%c.bmp", c);
        chars->surfaces[c] = SDL_LoadBMP(filename);
        if (!chars->surfaces[c]) {
            printf("Failed to load %c.bmp: %s\n", c, SDL_GetError());
            return false;
        
}
        SDL_SetColorKey(chars->surfaces[c], SDL_TRUE,
            SDL_MapRGB(chars->surfaces[c]->format, 0, 0, 0));
        chars->textures[c] = SDL_CreateTextureFromSurface(renderer, chars->surfaces[c]);
        SDL_FreeSurface(chars->surfaces[c]);
    
}
    struct SpecialChar {
        char character;
        const char* filename;
    
} specialChars[] = {
        {'!', "assets/font/bmp/exclamation.bmp"
},
        {':', "assets/font/bmp/colon.bmp"
},
        {'.', "assets/font/bmp/dot.bmp"
},
        {',', "assets/font/bmp/comma.bmp"
},
        {'?', "assets/font/bmp/question.bmp"
}
    
};
    for (size_t i = 0; i < sizeof(specialChars) / sizeof(specialChars[0]); i++) {
        chars->surfaces[specialChars[i].character] = SDL_LoadBMP(specialChars[i].filename);
        if (!chars->surfaces[specialChars[i].character]) {
            printf("Failed to load %s: %s\n", specialChars[i].filename, SDL_GetError());
            return false;
        
}
        SDL_SetColorKey(chars->surfaces[specialChars[i].character], SDL_TRUE,
            SDL_MapRGB(chars->surfaces[specialChars[i].character]->format, 0, 0, 0));
        chars->textures[specialChars[i].character] =
            SDL_CreateTextureFromSurface(renderer, chars->surfaces[specialChars[i].character]);
        SDL_FreeSurface(chars->surfaces[specialChars[i].character]);
    
}
    return true;
}
void drawText(const char* text, int x, int y, characterBitmaps* chars, SDL_Renderer* renderer) {
    int currentX = x;
    for (const char* c = text; *c != '\0'; c++) {
        if (*c == ' ') {
            currentX += 16;
            continue;
        
}
        if (chars->textures[*c] != NULL) {
            int width, height;
            SDL_QueryTexture(chars->textures[*c], NULL, NULL, &width, &height);
            SDL_Rect dstRect = {
                currentX,
                y,
                width,
                height
            
};
            SDL_RenderCopy(renderer, chars->textures[*c], NULL, &dstRect);
            currentX += width + 2;
        
}
    
}
}
void cleanupCharacterBitmaps(characterBitmaps* chars) {
    for (int i = 0; i < 128; i++) {
        if (chars->textures[i] != NULL) {
            SDL_DestroyTexture(chars->textures[i]);
            chars->textures[i] = NULL;
        
}
    
}
}
