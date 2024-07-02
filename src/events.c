#include "events.h"
#include "board.h"
#include <stdbool.h>

void handleMouseEventsOnBoard(SDL_Event* e) {

    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
        printf("left button down x: %d, y: %d \n", e->button.x, e->button.y);

        setCurrentSquareFromUI(e->button.x, e->button.y);

        return;
    }

    if (e->type == SDL_MOUSEMOTION) {
        tracePieceWithMouse(e->button.x, e->button.y);

        return;
    }
    
    if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT) {
        printf("settle button down x: %d, y: %d \n", e->button.x, e->button.y);

        settlePieceOnSquareFromUI(e->button.x, e->button.y);
        
        return;
    }
    
}