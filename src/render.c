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

void initTextures(SDL_Renderer* renderer) {
    pieceSprites = loadTexture(SPRITES_PATH, renderer);
    chessboardTexture = loadTexture(CHESSBOARD_IMAGE_PATH, renderer);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, UNIT, UNIT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    Uint32 greyishGreen = SDL_MapRGB(surface->format, 169, 169, 129);
    SDL_FillRect(surface, NULL, greyishGreen);

    selectedBgTile  = SDL_CreateTextureFromSurface(renderer, surface);

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

    BoardSquare* currentSquare = getCurrentSqaure();

    for (int i = 0; i < BOARD_LENGTH; i++) {
        
        BoardSquare* square = getSqaure(i % 8, i / 8);

        
        Piece* currPiece = square->piece;
        

        if(currPiece != NULL && !square->isSelected) {

            SDL_RenderCopy(renderer, pieceSprites, getClipRectForPiece(currPiece), &(currPiece->rectangle));
        }

    }

    if(currentSquare != NULL) {
        Coordinates currentCoords = transformToScreenCoordinates(currentSquare->index % 8, currentSquare->index /8);

        SDL_RenderCopy(renderer, selectedBgTile, NULL, &(((SDL_Rect){ currentCoords.x, currentCoords.y, UNIT, UNIT })));
        SDL_RenderCopy(renderer, pieceSprites, getClipRectForPiece(currentSquare->piece), &(currentSquare->piece->rectangle));
    }

    SDL_RenderPresent(renderer);
}