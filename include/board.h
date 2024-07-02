#pragma once 

#include "piece.h"
#include <stdbool.h>

typedef struct {
    Uint32 index;
    Piece* piece;
    bool isSelected;
    bool isMovedInto;
} BoardSquare;



BoardSquare* getSqaure(int x, int y);

BoardSquare* getCurrentSqaure(); 

void setPieceOnSquare(Piece* piece, int x, int y);

void setCurrentPieceOnSquare(int x, int y);


Piece* emptyPieceOnSquare(int x, int y);


void destroyPieceOnSquare(int x, int y);

void setCurrentSquareFromUI(int x, int y);


void tracePieceWithMouse(int x, int y);


bool landingOnIllegalSquare(int x, int y);


void settlePieceOnSquareFromUI(int x, int y);

void initBoard();