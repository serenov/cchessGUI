#include "board.h"
#include "config.h"
#include "piece.h"
#include "utils.h"
#include "interface.h"

#include <stdbool.h>

BoardSquare globalBoard[64];

BoardSquare* currentSelectedSquare = NULL;

SDL_Rect tracedSquare = { -100, -100, UNIT, UNIT };

LatestMove latestMove = { {-100, -100, UNIT, UNIT}, {-100, -100, UNIT, UNIT} };

BoardSquare* getSqaure(int x, int y) {
    int position = y * 8 + x;

    return (globalBoard + position);
}

BoardSquare* getCurrentSqaure() {
    return currentSelectedSquare;
}

void setPieceOnSquare(Piece_GUI* piece, int x, int y) {
    int position = y * 8 + x;

    globalBoard[position].piece = piece;
}

void setCurrentPieceOnSquare(int x, int y) {
    if(currentSelectedSquare == NULL) return;

    setPieceOnSquare(currentSelectedSquare->piece, x, y);
}

Piece_GUI* emptyPieceOnSquare(int x, int y) {
    int position = y * 8 + x;

    if(globalBoard[position].piece == NULL) return NULL;

    Piece_GUI* pieceAtSquare = globalBoard[position].piece;

    globalBoard[position].piece = NULL;

    return pieceAtSquare; 
}

void destroyPieceOnSquare(int x, int y) {
    Piece_GUI* unwantedPiece = emptyPieceOnSquare(x, y);

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

    Piece_GUI* currentPiece = currentSelectedSquare->piece;

    currentPiece->rectangle.x = x - (UNIT / 2);
    currentPiece->rectangle.y =  y - (UNIT / 2);

   tracedSquare.x = (x / UNIT) * UNIT; 
   tracedSquare.y = (y / UNIT) * UNIT; 
}

SDL_Rect* getTracedSquare() {
    if(currentSelectedSquare == NULL) return NULL;

    return &tracedSquare;
}

bool landingOnIllegalSquare(int x, int y, int screenX, int screenY) {
    int position = y * 8 + x;

    bool outOfBounds = (screenX > 800) || (screenX < 0) || (screenY > 800) || (screenY < 0);

    bool landedOnInitialPosition = (position == currentSelectedSquare->index);

    bool illegalSquare = !(ccInterfaceIsLegal(currentSelectedSquare->index, position));

    return outOfBounds || landedOnInitialPosition || illegalSquare;
}

void setLatestMove(int prevX, int prevY, int currX, int currY) {
    latestMove.moveFromSquare.x = prevX;
    latestMove.moveFromSquare.y = prevY;

    latestMove.moveToSquare.x = currX;
    latestMove.moveToSquare.y = currY;
}

LatestMove* getLatestMove() {

    return &latestMove;
}

void settlePieceOnSquareFromUI(int x, int y) {
    if(currentSelectedSquare == NULL) return;

    Coordinates boardCoords = transformToBoardCoordinates(x, y);

    Piece_GUI* currentPiece = currentSelectedSquare->piece;

    int previousPosition = currentSelectedSquare->index;
    Coordinates prevCoords = transformToScreenCoordinates(previousPosition % 8, previousPosition / 8);

    if(!landingOnIllegalSquare(boardCoords.x, boardCoords.y, x, y)) {

        destroyPieceOnSquare(boardCoords.x, boardCoords.y);
        setCurrentPieceOnSquare(boardCoords.x, boardCoords.y);

        emptyPieceOnSquare(previousPosition % 8, previousPosition / 8);

        // just to be consistent with the board coordinates.
        Coordinates screenCoords = transformToScreenCoordinates(boardCoords.x, boardCoords.y);

        currentPiece->rectangle.x = screenCoords.x;
        currentPiece->rectangle.y = screenCoords.y;

        setLatestMove(prevCoords.x, prevCoords.y, screenCoords.x, screenCoords.y);

        ccInterfacePlayMove(previousPosition, boardCoords.y * 8 + boardCoords.x, ' ');

        if (ccInterfaceLatestMoveType() == EnpassantMove) {

           Color previousTurnColor = ccGetColor(false);

           int enpassantY = (previousTurnColor == white)? -1: 1;

           destroyPieceOnSquare(boardCoords.x, boardCoords.y + enpassantY);
        }
        

        // switch (ccInterfaceLatestMoveType())
        // {
        // case RegularMove:
        //     printf("It is a regular move!!!!\n");
        //     break;

        // case EnpassantMove:
        //     printf("It is an Enpassant move.\n");
        //     break;

        // default:
        //     printf("It is an unknown move don't know what went wrong.\n");
        //     break;
        // }

    }
    else {
        currentPiece->rectangle.x = prevCoords.x;
        currentPiece->rectangle.y = prevCoords.y; 
    }

    currentSelectedSquare->isSelected = false;
    
    currentSelectedSquare = NULL;
    tracedSquare.x = -100; 
    tracedSquare.y = -100; 
}

Piece_GUI* generatePiece(PieceType_GUI type, Color_GUI clr, int x, int y) {
    Piece_GUI* piece = (Piece_GUI*) malloc(sizeof(Piece_GUI));

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
    Piece_GUI* generatedPiece = NULL;
    switch (pieceChar)
    {
    case 'k':
        generatedPiece = generatePiece(King_GUI, Black, x, y);
        break;
    
    case 'q':
        generatedPiece = generatePiece(Queen_GUI, Black, x, y);
        break;
    
    case 'r':
        generatedPiece = generatePiece(Rook_GUI, Black, x, y);
        break;
    
    case 'b':
        generatedPiece = generatePiece(Bishop_GUI, Black, x, y);
        break;
    
    case 'n':
        generatedPiece = generatePiece(Knight_GUI, Black, x, y);
        break;
    
    case 'p':
        generatedPiece = generatePiece(Pawn_GUI, Black, x, y);
        break;

    case 'K':
        generatedPiece = generatePiece(King_GUI, White, x, y);
        break;
    
    case 'Q':
        generatedPiece = generatePiece(Queen_GUI, White, x, y);
        break;
    
    case 'R':
        generatedPiece = generatePiece(Rook_GUI, White, x, y);
        break;
    
    case 'B':
        generatedPiece = generatePiece(Bishop_GUI, White, x, y);
        break;
    
    case 'N':
        generatedPiece = generatePiece(Knight_GUI, White, x, y);
        break;
    
    case 'P':
        generatedPiece = generatePiece(Pawn_GUI, White, x, y);
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
