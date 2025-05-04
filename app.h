// app.h
#ifndef APP_H
#define APP_H

#include "constants.h"
#include "paddle.h"
#include "ball.h"

// SDL static variables
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

// Possible Game Modes
typedef enum
{
    PVP = 0U,
    PVE = 1U,
} GameMode;

// Possible states the game can be in
typedef enum
{
    RUNNING =   0U, // Game is running 
    PAUSED =    1U, // Pause menu
} GameState;

// Contains game data and SDL constants like window and renderer
typedef struct
{
    SDL_Window   *window;
    SDL_Renderer *renderer;
    Paddle       *left_paddle;
    Paddle       *right_paddle;
    Ball         *ball;
    GameState    state;                      
    GameMode     game_mode;
    Uint64       pause_time;
    Uint64       last_step;
} AppState;

// Map starting positions for each paddle
static const int p_starting_positions[PLAYER_COUNT][2] = {
    {2,GAME_HEIGHT/3},            // Player 1
    {GAME_WIDTH-3,GAME_HEIGHT/3}  // Player 2
};

// Map Up/Down keys for each player
static const SDL_Scancode player_keys[PLAYER_COUNT][2] = {
    {SDL_SCANCODE_UP, SDL_SCANCODE_DOWN},  // Player 1
    {SDL_SCANCODE_W, SDL_SCANCODE_S}       // Player 2
};


// Colors
static const SDL_Color COLOR_BG                   = {30, 30, 30, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_DIVIDER              = {250, 250, 250, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_SCORE                = {250, 250, 250, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_BALL                 = {230, 230, 230, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_P1                   = {80, 180, 180, SDL_ALPHA_OPAQUE};   
static const SDL_Color COLOR_P2                   = {220, 150, 80, SDL_ALPHA_OPAQUE};

// Function declarations
static SDL_AppResult handle_key_up_event(void *appstate, SDL_Scancode key_code);
static SDL_AppResult handle_key_down_event(void *appstate, SDL_Scancode key_code);
void render_game(void *appstate);
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]);
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
SDL_AppResult SDL_AppIterate(void *appstate);
void SDL_AppQuit(void *appstate, SDL_AppResult result);

#endif