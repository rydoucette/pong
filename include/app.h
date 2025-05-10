// app.h
#ifndef APP_H
#define APP_H

#include <SDL3_ttf/SDL_ttf.h>
#include "constants.h"
#include "gamestate_stack.h"
#include "paddle.h"
#include "ball.h"

// Possible Game Modes
typedef enum
{
    PVP = 0U,
    PVE = 1U,
} GameMode;

// states for debug
typedef enum {
    PLAY,
    GAME_OVER,
    PAUSE,
    START_MENU
} StateId;

// Contains game data and SDL constants like window and renderer
typedef struct AppState 
{
    SDL_Window     *window;
    SDL_Renderer   *renderer;
    TTF_Font       *score_font;
    TTF_Font       *message_font;
    TTF_Font       *small_message_font;
    TTF_Font       *option_font;
    TTF_Font       *title_font;
    Paddle         *left_paddle;
    Paddle         *right_paddle;
    Ball           *ball;
    GameStateStack *game_state_stack;                      
    GameMode       game_mode;
    Uint64         last_step;
} AppState;

// Map starting positions for each paddle
static const int p_starting_positions[PLAYER_COUNT][2] = {
    {0,GAME_HEIGHT/3},            // Player 1
    {GAME_WIDTH-1,GAME_HEIGHT/3}  // Player 2
};

static const int score_positions[PLAYER_COUNT][2] = {
    {39*SDL_WINDOW_WIDTH/100,20}, // Player 1
    {53*SDL_WINDOW_WIDTH/100,20}  // Player 2
};

// Map Up/Down keys for each player
static const SDL_Scancode player_keys[PLAYER_COUNT][2] = {
    {SDL_SCANCODE_UP, SDL_SCANCODE_DOWN},  // Player 1
    {SDL_SCANCODE_W, SDL_SCANCODE_S}       // Player 2
};


// Colors
static const SDL_Color COLOR_BG                   = {17, 17, 17, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_START_MENU      = {35, 30, 60, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_P1                   = {0, 200, 255, SDL_ALPHA_OPAQUE};   
static const SDL_Color COLOR_P2                   = {255, 100, 100, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_BALL                   = {190, 170, 160, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_WINNER               = {255, 215, 0, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_LOSER                = {105, 105, 105, SDL_ALPHA_OPAQUE};

// Text colors
static const SDL_Color COLOR_SCORE                = {250, 250, 250, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_TITLE                = {255, 255, 255, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_STATE                = {250, 250, 250, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_MESSAGE_OFF          = {120, 120, 180, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_MESSAGE_ON           = {200, 200, 255, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_OPTION_DISABLED      = {60, 60, 100, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_OPTION_HIGHLIGHTED   = {100, 170, 255, SDL_ALPHA_OPAQUE};

// Function declarations
static SDL_AppResult handle_key_up_event(void *appstate, SDL_Scancode key_code);
static SDL_AppResult handle_key_down_event(void *appstate, SDL_Scancode key_code);
void render_game(void *appstate);
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]);
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
SDL_AppResult SDL_AppIterate(void *appstate);
void SDL_AppQuit(void *appstate, SDL_AppResult result);

#endif