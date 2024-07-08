#include "config.h"

#include <cchess/engine.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

extern Boards __boards__;
void displayBoard()
{
    for (int y = 7; y > -1; y--)
    {

        printf("\n -------------------------------\n");
        for (int x = 0; x < 8; x++)
        {
            printf("| %c ", __boards__.board[y * 8 + x]);
        }
        printf("|");
    }
    printf("\n -------------------------------\n");

    // displayBitboard(__boards__.bitboards.occupiedBoard);
    // displayBitboard(__boards__.bitboards.whitePieces);
    // displayBitboard(__boards__.bitboards.blackPieces);
    // displayBitboard(__boards__.bitboards.occupiedRotated90LBoard);
    // displayBitboard(__boards__.bitboards.occupiedRotated45LBoard);
    // displayBitboard(__boards__.bitboards.occupiedRotated45RBoard);
}

void ccInterfaceInit () {
    initMagicBoards(); 

    if(!setFEN(DEFAULT_FEN)) {
        printf("Improper FEN.");
        exit(1);
    };

    displayBoard();
}

bool ccInterfacePlayMove(int from, int to, char promotionToPiece) {
    if(!playMoveOnBoards(from, to, promotionToPiece)){
        return false;
    }

    return true;
}

Bitboard ccInterfaceListMoves(int position) {
    return getAllowedSquares(position);
}