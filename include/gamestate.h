#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SDL3/SDL.h>
struct AppState;
typedef struct AppState AppState;

/*
 * GameState is an interface for different screens (play, pause, menu, etc.).
 * Each state implements update, render, and event handling logic.
 */

typedef struct GameState {
    void (*update)(void *appstate);
    void (*render)(void *appstate);
    SDL_AppResult (*handle_event)(void *appstate, SDL_Event *event);
    int state_id; //debug
} GameState;

// Factory functions for specific game states
GameState *create_play_state(void);
GameState *create_game_over_state(void);
GameState *create_pause_state(void);
GameState *create_start_menu_state(void);

#endif