// app.h
#ifndef APP_H
#define APP_H

#include <SDL3_ttf/SDL_ttf.h>
#include "constants.h"
#include "gamestate_stack.h"
#include "paddle.h"
#include "ball.h"


// map players to scancodes
extern const SDL_Scancode player_keys[PLAYER_COUNT][2];

// Possible Game Modes
typedef enum {
    PVP,
    PVE,
} GameMode;

typedef struct {
    float ball_speed;
    float ai_reaction_time;
    float ai_inaccuracy;  // 0.0 = perfect, higher = more error
} DifficultySettings;
extern DifficultySettings EASY_SETTINGS;
extern DifficultySettings MEDIUM_SETTINGS;
extern DifficultySettings HARD_SETTINGS;
extern DifficultySettings DEMO_SETTINGS;

// Possible Game Modes
typedef enum {
    EASY,
    MEDIUM,
    HARD,
    DEMO
} DifficultyLevel;

// state id attached to each state in the stack. Used for debug only
typedef enum {
    PLAY,
    GAME_OVER,
    PAUSE,
    START_MENU
} StateId;

// Contains game data and SDL objects such as window and renderer
typedef struct AppState  {
    SDL_Window        *window;
    SDL_Renderer      *renderer;
    TTF_Font          *score_font;
    TTF_Font          *message_font;
    TTF_Font          *small_message_font;
    TTF_Font          *option_font;
    TTF_Font          *title_font;
    Paddle            *left_paddle;
    Paddle            *right_paddle;
    Ball              *ball;
    GameStateStack    *game_state_stack;                      
    GameMode            game_mode;
    DifficultyLevel     difficulty_level;
    DifficultyLevel     highlighted_difficulty_level;
    DifficultySettings *difficulty_settings;
    Uint64              last_step;
} AppState;

// Function declarations
SDL_AppResult handle_key_up_event(void *appstate, SDL_Scancode key_code);
SDL_AppResult handle_key_down_event(void *appstate, SDL_Scancode key_code);
void render_game(void *appstate);
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]);
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
SDL_AppResult SDL_AppIterate(void *appstate);
void SDL_AppQuit(void *appstate, SDL_AppResult result);
void cleanup_appstate(AppState *as);
TTF_Font* load_font(const char *path, int size);

#endif