#include "config.h"
#include "piece.h"
#include "utils.h"

#include <stdbool.h>

typedef struct {
    Uint32 index;
    Piece* piece;
    bool isSelected;
    bool isMovedInto;
} BoardSquare;

BoardSquare globalBoard[64];

BoardSquare* currentSelectedSquare = NULL;

BoardSquare* getSqaure(int x, int y) {
    int position = y * 8 + x;

    return (globalBoard + position);
}

BoardSquare* getCurrentSqaure() {
    return currentSelectedSquare;
}

void setPieceOnSquare(Piece* piece, int x, int y) {
    int position = y * 8 + x;

    globalBoard[position].piece = piece;
}

void setCurrentPieceOnSquare(int x, int y) {
    if(currentSelectedSquare == NULL) return;

    setPieceOnSquare(currentSelectedSquare->piece, x, y);
}

Piece* emptyPieceOnSquare(int x, int y) {
    int position = y * 8 + x;

    if(globalBoard[position].piece == NULL) return NULL;

    Piece* pieceAtSquare = globalBoard[position].piece;

    globalBoard[position].piece = NULL;

    return pieceAtSquare; 
}

void destroyPieceOnSquare(int x, int y) {
    Piece* unwantedPiece = emptyPieceOnSquare(x, y);

    if(unwantedPiece == NULL) return; 

    free(unwantedPiece);
}

void setCurrentSquareFromUI(int x, int y) {
    Coordinates transformedCoords = transformToBoardCoordinates(x, y);

    int position = transformedCoords.y * 8 + transformedCoords.x;

    if(globalBoard[position].piece == NULL) return;

    currentSelectedSquare = globalBoard + position;

    currentSelectedSquare->isSelected = true;
}

void tracePieceWithMouse(int x, int y) {
    if(currentSelectedSquare == NULL) return;

    Piece* currentPiece = currentSelectedSquare->piece;

    currentPiece->rectangle.x = x - (UNIT / 2);
    currentPiece->rectangle.y = y - (UNIT / 2);
}

bool landingOnIllegalSquare(int x, int y, int screenX, int screenY) {
    int position = y * 8 + x;

    bool outOfBounds = (screenX > 800) || (screenX < 0) || (screenY > 800) || (screenY < 0);

    bool landedOnInitialPosition = (position == currentSelectedSquare->index);

    return outOfBounds || landedOnInitialPosition;
}

void settlePieceOnSquareFromUI(int x, int y) {
    if(currentSelectedSquare == NULL) return;

    Coordinates boardCoords = transformToBoardCoordinates(x, y);

    Piece* currentPiece = currentSelectedSquare->piece;

    int previousPosition = currentSelectedSquare->index;

    if(!landingOnIllegalSquare(boardCoords.x, boardCoords.y, x, y)) {

        destroyPieceOnSquare(boardCoords.x, boardCoords.y);
        setCurrentPieceOnSquare(boardCoords.x, boardCoords.y);

        emptyPieceOnSquare(previousPosition % 8, previousPosition / 8);

        // just to be consistent with the board coordinates.
        Coordinates screenCoords = transformToScreenCoordinates(boardCoords.x, boardCoords.y);

        currentPiece->rectangle.x = screenCoords.x;
        currentPiece->rectangle.y = screenCoords.y;
    }
    else {
        Coordinates prevCoords = transformToScreenCoordinates(previousPosition % 8, previousPosition / 8);

        currentPiece->rectangle.x = prevCoords.x;
        currentPiece->rectangle.y = prevCoords.y; 
    }

    currentSelectedSquare->isSelected = false;
    
    currentSelectedSquare = NULL;
}

Piece* generatePiece(PieceType type, Color clr, int x, int y) {
    Piece* piece = (Piece*) malloc(sizeof(Piece));

    piece->clr = clr;

    piece->type = type;

    piece->rectangle.h = PIECE_SIZE;
    piece->rectangle.w = PIECE_SIZE;

    Coordinates screenCoords = transformToScreenCoordinates(x, y);

    piece->rectangle.x = screenCoords.x;
    piece->rectangle.y = screenCoords.y;

    return piece;
}

void sanityCheckFEN() {
    return;
}

void createAndPutPiece(char pieceChar, int x, int y) {
    Piece* generatedPiece = NULL;
    switch (pieceChar)
    {
    case 'k':
        generatedPiece = generatePiece(King, Black, x, y);
        break;
    
    case 'q':
        generatedPiece = generatePiece(Queen, Black, x, y);
        break;
    
    case 'r':
        generatedPiece = generatePiece(Rook, Black, x, y);
        break;
    
    case 'b':
        generatedPiece = generatePiece(Bishop, Black, x, y);
        break;
    
    case 'n':
        generatedPiece = generatePiece(Knight, Black, x, y);
        break;
    
    case 'p':
        generatedPiece = generatePiece(Pawn, Black, x, y);
        break;

    case 'K':
        generatedPiece = generatePiece(King, White, x, y);
        break;
    
    case 'Q':
        generatedPiece = generatePiece(Queen, White, x, y);
        break;
    
    case 'R':
        generatedPiece = generatePiece(Rook, White, x, y);
        break;
    
    case 'B':
        generatedPiece = generatePiece(Bishop, White, x, y);
        break;
    
    case 'N':
        generatedPiece = generatePiece(Knight, White, x, y);
        break;
    
    case 'P':
        generatedPiece = generatePiece(Pawn, White, x, y);
        break;
    
     
    default:
        break;
    }

    setPieceOnSquare(generatedPiece, x, y);
}


void parseFEN() {
    
    char* FEN = DEFAULT_FEN;

    int squareIndex = 56;

    for(int i = 0; *(FEN + i) != ' '; i++) {
        char currentChar = *(FEN + i);

        if(currentChar == '/') {

            squareIndex -= 16;
        }
        else if(currentChar >= '1' && currentChar <= '8') {
            squareIndex += currentChar - '0';
        }
        else {
            createAndPutPiece(currentChar, squareIndex % 8, squareIndex / 8);
            squareIndex++;
        }

    }
}

void initBoard() {
    for(int i = 0; i < BOARD_LENGTH; i++) {

        globalBoard[i].index = i; 
        globalBoard[i].piece = NULL; 
        globalBoard[i].isSelected = false; 
        globalBoard[i].isMovedInto = false; 
    }
    parseFEN();
}
