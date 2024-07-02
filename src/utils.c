#include "utils.h"
#include "config.h"

Coordinates transformToScreenCoordinates(int x, int y) {
    int transformedX = x * UNIT;
    int transformedY = SCREEN_SIZE - ((y + 1) * UNIT);

    return (Coordinates) { transformedX, transformedY };
}

Coordinates transformToBoardCoordinates(int x, int y) {
    int transformedX = x / UNIT;
    int transformedY = (SCREEN_SIZE - y) / UNIT;

    Coordinates transformedCoords = {transformedX, transformedY};

    return transformedCoords;
}



