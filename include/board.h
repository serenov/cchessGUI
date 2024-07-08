#pragma once 

#include "piece.h"
#include <stdbool.h>

typedef struct {
    Uint32 index;
    Piece_GUI* piece;
    bool isSelected;
    bool isMovedInto;
} BoardSquare;
typedef struct {
    SDL_Rect moveFromSquare;
    SDL_Rect moveToSquare;

} LatestMove;

LatestMove* getLatestMove(); 

BoardSquare* getSqaure(int x, int y);

BoardSquare* getCurrentSqaure(); 

SDL_Rect* getTracedSquare(); 

void setPieceOnSquare(Piece_GUI* piece, int x, int y);

void setCurrentPieceOnSquare(int x, int y);

Piece_GUI* emptyPieceOnSquare(int x, int y);

void destroyPieceOnSquare(int x, int y);

void setCurrentSquareFromUI(int x, int y);

void tracePieceWithMouse(int x, int y);

bool landingOnIllegalSquare(int x, int y, int screenX, int screenY);

void settlePieceOnSquareFromUI(int x, int y);

void initBoard();