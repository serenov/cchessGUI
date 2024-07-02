#include "sprites.h"
#include "config.h"

int spriteOffsets[][2] = {
    { 910, 430 },
    { 610, 430 },
    { 10, 430 },
    { 310, 430 },
    { 1210, 430 },
    { 1510, 430 },
    { 910, 55 },
    { 610, 55 },
    { 10, 55 },
    { 310, 55 },
    { 1210, 55 },
    { 1510, 55 },
};

SDL_Rect globalRect = { 0, 0, SPRITE_FRAME_SIZE, SPRITE_FRAME_SIZE };

SDL_Rect *getClipRectForPiece(Piece *piece) {
    int index = piece->type + piece->clr * 6;
    
    globalRect.x = spriteOffsets[index][0];
    globalRect.y = spriteOffsets[index][1];

    return &globalRect;
}

// void getClipRectForPiece1() {
//     printf("{\n");

//     for(int i = 0; i < 12; i++) {
//         int pieceOffset;

//         switch (i % 6)
//         {
//         case King:
//             pieceOffset = 3;  
//             break;

//         case Queen:
//             pieceOffset = 2;  
//             break;
        
//         case Rook:
//             pieceOffset = 0;  
//             break;
    
//         case Bishop:
//             pieceOffset = 1;  
//             break;
    
//         case Knight:
//             pieceOffset = 4;  
//             break;
    
//         case Pawn:
//             pieceOffset = 5;  
//             break;
    
//         default:
//             break;
//         }


//         int pieceColor = i / 6;

//         int rectX = SPRITE_X_SKIP_SIZE * pieceOffset + SPRITE_X_OFFSET;
//         int rectY = SPRITE_Y_SKIP_SIZE * pieceColor + SPRITE_Y_OFFSET;

//         printf("\t{ %d, %d },\n", rectX, rectY);
//     }
//     printf("}\n");
// }