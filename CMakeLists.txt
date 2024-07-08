cmake_minimum_required(VERSION 3.10)

# Set the project name
project(SDL_Chessboard)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add the include directory
include_directories(include)

# Add the source files
set(SOURCES
    src/utils.c
    src/sprites.c
    src/events.c
    src/board.c
    src/render.c
    src/main.c
)

# Find SDL2 package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Find SDL2_image package
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

find_library(CCHESS_LIBRARY cchess PATHS /usr/local/lib)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Manually specify libraries
target_link_libraries(${PROJECT_NAME} -lSDL2 -lSDL2_image ${CCHESS_LIBRARY})


