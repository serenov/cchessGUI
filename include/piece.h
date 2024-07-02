#pragma once

#include <SDL2/SDL.h>

typedef enum {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn
} PieceType;

typedef struct {
    int x, y;
} Coordinates;

typedef enum {
    White,
    Black
} Color;

typedef struct {
    Color clr;
    PieceType type;
    SDL_Rect rectangle;
} Piece;