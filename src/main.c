#include <SDL2/SDL.h>
#include <stdio.h>
#include "render.h"
#include "config.h"
#include "events.h"
#include "interface.h"

const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

// Screen dimensions
int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    ccInterfaceInit();

    // Initialize SDL and create window and renderer
    if (!init(&window, &renderer, SCREEN_WIDTH, SCREEN_HEIGHT)) {
        printf("Failed to initialize!\n");
        return -1;
    }

    // Main loop flag
    int quit = 0;
    // Event handler
    SDL_Event e;

    // Main loop
    while (!quit) {

        Uint32 frameStart = SDL_GetTicks();
        
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = 1;
            }

            handleMouseEventsOnBoard(&e);
        }

        renderTheGame(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    // Destroy window and renderer
    closeSDL(window, renderer);

    return 0;
}
