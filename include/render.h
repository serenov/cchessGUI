#pragma once

#include "board.h"
#include <SDL2/SDL.h>

// Function declarations
int init(SDL_Window** window, SDL_Renderer** renderer, int width, int height);
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer);
void closeSDL(SDL_Window* window, SDL_Renderer* renderer);
void renderTheGame(SDL_Renderer* renderer);
SDL_Texture* getTextureForPiece(PieceType_GUI piece); 