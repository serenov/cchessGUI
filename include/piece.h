#pragma once

#include <SDL2/SDL.h>

typedef enum {
    King_GUI,
    Queen_GUI,
    Rook_GUI,
    Bishop_GUI,
    Knight_GUI,
    Pawn_GUI
} PieceType_GUI;
typedef struct {
    int x, y;
} Coordinates;

typedef enum {
    White,
    Black
} Color_GUI;

typedef struct {
    Color_GUI clr;
    PieceType_GUI type;
    SDL_Rect rectangle;
} Piece_GUI;