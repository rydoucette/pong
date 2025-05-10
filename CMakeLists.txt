cmake_minimum_required(VERSION 3.16)
project(pong)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Check if we're building Debug or Release
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
    # Debug-specific settings
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")  # Add debugging symbols
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")  # Add debugging symbols
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
    # Release-specific settings
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")  # Optimization for release
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")  # Optimization for release
endif()

# Use vendored libs
set(SDLTTF_VENDORED ON)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# This assumes the SDL_ttf source is available in vendored/SDL_ttf
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

# This assumes the SDL_image source is available in vendored/SDL_image
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Create your game executable target as usual
add_executable(pong
    src/app.c
    src/game_logic.c
    src/render.c
    src/paddle.c
    src/ball.c
    src/score.c
    src/gamestate_stack.c
    src/states/play_state.c
    src/states/game_over_state.c
    src/states/pause_state.c
    src/states/start_menu_state.c
)

# Link to the actual SDL3 library.
target_link_libraries(pong PRIVATE m SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf SDL3::SDL3)