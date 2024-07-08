#include "board.h"
#include "config.h"
#include "render.h"
#include "sprites.h"
#include "utils.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdio.h>

SDL_Texture* pieceSprites = NULL; 
SDL_Texture* chessboardTexture = NULL; 
SDL_Texture* selectedBgTile = NULL; 
SDL_Texture* hoveredBgTile = NULL; 
SDL_Texture* latestMoveTile = NULL;

void initTextures(SDL_Renderer* renderer) {
    SDL_Cursor* cursor;
    cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    SDL_SetCursor(cursor);

    pieceSprites = loadTexture(SPRITES_PATH, renderer);
    chessboardTexture = loadTexture(CHESSBOARD_IMAGE_PATH, renderer);
    hoveredBgTile = loadTexture(HOVER_BG, renderer);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, UNIT, UNIT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    Uint32 green = SDL_MapRGB(surface->format, 129, 150, 105);
    Uint32 lightGreen = SDL_MapRGB(surface->format, 155, 199, 0);

    SDL_FillRect(surface, NULL, green);
    selectedBgTile  = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FillRect(surface, NULL, lightGreen);
    latestMoveTile  = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_SetTextureBlendMode(latestMoveTile, SDL_BLENDMODE_BLEND);

    // Set the alpha value for the transparent texture (50% transparency)
    SDL_SetTextureAlphaMod(latestMoveTile, 128);

    SDL_FreeSurface(surface);
}

// Initializes SDL, the window, and the renderer
int init(SDL_Window** window, SDL_Renderer** renderer, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    *window = SDL_CreateWindow("SDL Chessboard",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               width, height,
                               SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (*renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        return 0;
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;

    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        return 0;
    }

    initBoard();
    initTextures(*renderer);

    return 1;
}

// Loads a texture from a specified path
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return NULL;
    }

    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
    }

    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

// Cleans up and closes SDL, the window, and the renderer
void closeSDL(SDL_Window* window, SDL_Renderer* renderer) {
    // destroyBoard();    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

// SDL_Rect spriteRect = { parameterX,  parameterY,  300, 300 };

void renderTheGame(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, chessboardTexture, NULL, NULL);

    SDL_Rect* tracedSquare = getTracedSquare();
    BoardSquare* currentSquare = getCurrentSqaure();

    if (tracedSquare != NULL) {
        SDL_RenderCopy(renderer, hoveredBgTile, NULL, tracedSquare);
    }

    if (currentSquare == NULL) {
        LatestMove* latestMove = getLatestMove();

        SDL_RenderCopy(renderer, latestMoveTile, NULL, &(latestMove->moveFromSquare));
        SDL_RenderCopy(renderer, latestMoveTile, NULL, &(latestMove->moveToSquare));
    }

    for (int i = 0; i < BOARD_LENGTH; i++) {
        
        BoardSquare* square = getSqaure(i % 8, i / 8);

        
        Piece* pieceAtSquare = square->piece;
        

        if(pieceAtSquare != NULL && !square->isSelected) {

            SDL_RenderCopy(renderer, pieceSprites, getClipRectForPiece(pieceAtSquare), &(pieceAtSquare->rectangle));
        }

    }

    if(currentSquare != NULL) {
        Coordinates currentCoords = transformToScreenCoordinates(currentSquare->index % 8, currentSquare->index /8);

        SDL_RenderCopy(renderer, selectedBgTile, NULL, &(((SDL_Rect){ currentCoords.x, currentCoords.y, UNIT, UNIT })));
        SDL_RenderCopy(renderer, pieceSprites, getClipRectForPiece(currentSquare->piece), &(currentSquare->piece->rectangle));
    }

    SDL_RenderPresent(renderer);
}