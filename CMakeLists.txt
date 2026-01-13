cmake_minimum_required(VERSION 3.16)
project(tetris)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(
    tetris 
    
    main.c 
    shapes.c
    board.c
    player.c
)

# Link to the actual SDL3 library.
target_link_libraries(
    tetris PRIVATE 
    SDL3_image::SDL3_image
    SDL3::SDL3)
