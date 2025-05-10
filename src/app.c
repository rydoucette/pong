#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>
#include "app.h"
#include "constants.h"
#include "gamestate_stack.h"
#include "paddle.h"
#include "game_logic.h"
#include "render.h"
#include "ball.h"

// This function runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState *as = (AppState *)appstate;

    GameState *current;
    if (!gamestate_stack_peek(as->game_state_stack, &current)){
        return SDL_APP_SUCCESS;
    } 
    return current->handle_event(as, event);
}

// This function runs once at startup
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {    
    
    // AppState stores various game specific information
    AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
    if (!as) {
        return SDL_APP_FAILURE;
    }
    *appstate = as;

    /* SDL */
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Create the window and renderer */
    if (!SDL_CreateWindowAndRenderer("PONG", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, 
                                     0, &as->window, &as->renderer)) {
        SDL_Log("Could not create window and render: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Metadata */
    if (!SDL_SetAppMetadata("TRON", "1.0", "TRONv1.0")) {
        return SDL_APP_FAILURE;
    }

    /* SDL_ttf */
    if (!TTF_Init()) {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    as->score_font = TTF_OpenFont("assets/fonts/Audiowide-Regular.ttf", 64);
    if (!as->score_font) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    as->message_font = TTF_OpenFont("assets/fonts/Audiowide-Regular.ttf", 24);
    if (!as->message_font) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    as->small_message_font = TTF_OpenFont("assets/fonts/Audiowide-Regular.ttf", 16);
    if (!as->small_message_font) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    as->title_font = TTF_OpenFont("assets/fonts/Audiowide-Regular.ttf", 64);
    if (!as->title_font) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    as->option_font = TTF_OpenFont("assets/fonts/Audiowide-Regular.ttf", 32);
    if (!as->option_font) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    as->game_state_stack = malloc(sizeof(GameStateStack));
    gamestate_stack_create(as->game_state_stack, GAME_STATE_STACK_SIZE);
    gamestate_stack_push(as->game_state_stack, create_start_menu_state());
    initialize_paddles(as, false, false); // sets up two AI players playing in background
    initialize_ball(as);
    return SDL_APP_CONTINUE;
}

// This function runs once per frame, and is the heart of the program
SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *as = (AppState *)appstate;
    const Uint64 now = SDL_GetTicks();

    GameState *current;
    if (!gamestate_stack_peek(as->game_state_stack, &current)) return SDL_APP_SUCCESS;
    while(now - as->last_step >= STEP_RATE_IN_MILLISECONDS) {
        current->update(as);
        as->last_step += STEP_RATE_IN_MILLISECONDS;
    }
    current->render(as);

    return SDL_APP_CONTINUE;
}

// This function runs once at shutdown
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (appstate != NULL) {
        AppState *as = (AppState *)appstate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        TTF_CloseFont(as->score_font);
        TTF_CloseFont(as->message_font);
        free_paddle(as->left_paddle);
        free_paddle(as->right_paddle);
        free_ball(as->ball);
        SDL_free(as);
    }
}