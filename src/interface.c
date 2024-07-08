#include <cchess/engine.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum {
    RankError,
    FileError,
    lengthError,
    illegalMoveError,
    Check,
    CheckmateOnBoard,
    StalemateOnBoard,
    allFine

} InterfaceInformation;

extern Boards __boards__;

#define BUFFER_LENGTH 5
#define MIN_LENGTH 4
#define NEWLINE() printf("\n");

#ifdef _WIN32
    // Windows
    #define CLEAR_SCREEN() system("cls")
#else
    // Unix-like systems
    #define CLEAR_SCREEN() printf("\033[H\033[2J")
#endif

void initBuffer(char *buffer)
{
    for(int i = 0; i < BUFFER_LENGTH; i++) buffer[i] = 0;
    
}

inline static bool getBufferAndValidateLength(char *buffer)
{
    int count = 0;
    char c;
    while((c = getchar()) != '\n')
    {
        if(count >= BUFFER_LENGTH) return false;
        buffer[count++] = c;
    }

    if(count < (MIN_LENGTH - 1)) return false;

    return true;
}

InterfaceInformation getInputAndValidate(char *buffer)
{
    if(!getBufferAndValidateLength(buffer))
    {
        return lengthError;
    } 

    for(int i = 0; i < 4; i++)
    {
        char currentChar = *(buffer++);

        if(i % 2) 
        {
            // ODD
            if(currentChar < '1' || currentChar > '8') 
            {
                return RankError;
            }
        }
        else 
        {
            // EVEN
            if(currentChar < 'a' || currentChar > 'h')
            {
                return FileError;
            }
        }
    }

    // if(*buffer != 0 && *buffer != '\n') {
    //     if(buffer)
    // }
    return allFine;
}

InterfaceInformation decodeAndPlay(char *input)
{
    // input must be 5 characters long
    // input must be valid i.e, in format [from square][to square][promotion info (optional)].
    
    Square from = input[0] - 'a' + (input[1] - '1') * 8;
    Square to = input[2] - 'a' + (input[3] - '1') * 8;

    if(!playMoveOnBoards(from, to, input[4]))
    {
        return illegalMoveError;
    }

    if(!isKingSafe()) return Check;


    return allFine;
}





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

void printInterfaceInformation(InterfaceInformation info, char* buffer)
{
    char *s = getColor(true) == white? "white": "black";

    switch (info)
    {
    case RankError:
        printf("[ERROR] Invalid rank.");
        break;
    case FileError:
        printf("[ERROR] Invalid rank input.");
        break;
    case illegalMoveError:
        printf("[ERROR] Illegal move.");
        break;
    case Check: 
        printf("[WARNING] %s's king is in check.", s);
        break;
    case CheckmateOnBoard:
        printf("[INFO] Checkmate, %s is triumphant.", s);
        return;
    case StalemateOnBoard:
        printf("[INFO] Match ended in an impasse.");
        return;
    
    default:
        break;
    }

    NEWLINE();

    if(info != allFine && info != Check) {
        // print last input incase if it's an illegal move
        // TODO: fix the logic here
        printf("[INFO] Last Input: ");

        while(*buffer) printf("%c", *(buffer++));

        NEWLINE();
    }

    printf("[INFO] Now %s's turn.", s);
}

void gameloop()
{
    char buffer[BUFFER_LENGTH];

    GameStatus gs;
    InterfaceInformation info = allFine;

    // clearing for the first render
    CLEAR_SCREEN();
    

    while((gs = getGameStatus()) == RUNNING)
    {
        printInterfaceInformation(info, buffer); 

        displayBoard();
        initBuffer(buffer);

        if((info = getInputAndValidate(buffer)) == allFine)
        {
            info = decodeAndPlay(buffer);
        }

        CLEAR_SCREEN();
    }

    CLEAR_SCREEN();

    displayBoard();

    if(gs == CHECKMATE) printInterfaceInformation(CheckmateOnBoard, NULL);
    else printInterfaceInformation(StalemateOnBoard, NULL);
}



int main(int argc, char *argv)
{
    initMagicBoards(); 

    if(!setFEN(NULL)) {
        printf("Improper FEN.");
        exit(1);
    };

    gameloop();

    // displayBitboard(__boards__.bitboards.blackKing);
    // displayBitboard(__boards__.bitboards.whiteKing);
    // displayBitboard(__boards__.bitboards.occupiedBoard);
    // displayBitboard(__boards__.bitboards.whitePieces);
    // displayBitboard(__boards__.bitboards.blackPieces);
    // displayBitboard(__boards__.bitboards.occupiedRotated90LBoard);
    // displayBitboard(__boards__.bitboards.occupiedRotated45LBoard);
    // displayBitboard(__boards__.bitboards.occupiedRotated45RBoard);

    return 0;
}