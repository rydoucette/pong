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

// map players to scancodes
const SDL_Scancode player_keys[PLAYER_COUNT][2] = {
    {SDL_SCANCODE_UP, SDL_SCANCODE_DOWN},
    {SDL_SCANCODE_W, SDL_SCANCODE_S}
};

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
        SDL_Log("Could not allocate memory for appstate: %s", SDL_GetError());
        cleanup_appstate(as);
        return SDL_APP_FAILURE;
    }
    *appstate = as;

    /* SDL */
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        cleanup_appstate(as);
        return SDL_APP_FAILURE;
    }

    /* Create the window and renderer */
    if (!SDL_CreateWindowAndRenderer("PONG", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, 
                                     0, &as->window, &as->renderer)) {
        SDL_Log("Could not create window and render: %s", SDL_GetError());
        cleanup_appstate(as);
        return SDL_APP_FAILURE;
    }

    /* Metadata */
    if (!SDL_SetAppMetadata("TRON", "1.0", "TRONv1.0")) {
        SDL_Log("Couldn't set metadata: %s\n", SDL_GetError());
        cleanup_appstate(as);
        return SDL_APP_FAILURE;
    }

    /* SDL_ttf */
    if (!TTF_Init()) {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        cleanup_appstate(as);
        return SDL_APP_FAILURE;
    }

    /* Load fonts */
    as->title_font = load_font(FONT_PATH, FONT_SIZE_TITLE);
    if (!as->title_font) { cleanup_appstate(as); return SDL_APP_FAILURE; }

    as->option_font = load_font(FONT_PATH, FONT_SIZE_OPTION);
    if (!as->option_font) { cleanup_appstate(as); return SDL_APP_FAILURE; }

    as->message_font = load_font(FONT_PATH, FONT_SIZE_MESSAGE);
    if (!as->message_font) { cleanup_appstate(as); return SDL_APP_FAILURE; }

    as->small_message_font = load_font(FONT_PATH, FONT_SIZE_SMALL_MESSAGE);
    if (!as->small_message_font) { cleanup_appstate(as); return SDL_APP_FAILURE; }

    as->score_font = load_font(FONT_PATH, FONT_SIZE_SCORE);
    if (!as->score_font) { cleanup_appstate(as); return SDL_APP_FAILURE; }

    /* Initialize some game values */
    as->game_state_stack = malloc(sizeof(GameStateStack));
    gamestate_stack_create(as->game_state_stack, GAME_STATE_STACK_SIZE);
    gamestate_stack_push(as->game_state_stack, create_start_menu_state());
    initialize_paddles(as, false, false); // sets up two AI players playing in background
    initialize_ball(as);
    as->game_mode = PVP;
    as->difficulty_level = EASY; // wont be relevant if user selects PVP

    return SDL_APP_CONTINUE;
}

// This function runs once per frame, and is the heart of the program
SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *as = (AppState *)appstate;
    const Uint64 now = SDL_GetTicks();

    GameState *state;
    if (!gamestate_stack_peek(as->game_state_stack, &state)) return SDL_APP_SUCCESS;
    while(now - as->last_step >= STEP_RATE_IN_MILLISECONDS) {
        state->update(as);
        as->last_step += STEP_RATE_IN_MILLISECONDS;
    }
    state->render(as);

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
        TTF_CloseFont(as->small_message_font);
        TTF_CloseFont(as->option_font);
        TTF_CloseFont(as->title_font);
        paddle_free(as->left_paddle);
        paddle_free(as->right_paddle);
        ball_free(as->ball);
        free(as->game_state_stack);
        TTF_Quit();
        SDL_Quit();
        SDL_free(as);
    }
}

// Gets called if anytthing fails to allocate memory and we are returning early
void cleanup_appstate(AppState *as) {
    if (!as) return;

    if (as->option_font)         TTF_CloseFont(as->option_font);
    if (as->title_font)          TTF_CloseFont(as->title_font);
    if (as->small_message_font)  TTF_CloseFont(as->small_message_font);
    if (as->message_font)        TTF_CloseFont(as->message_font);
    if (as->score_font)          TTF_CloseFont(as->score_font);

    if (as->game_state_stack)    free(as->game_state_stack);

    if (as->renderer)            SDL_DestroyRenderer(as->renderer);
    if (as->window)              SDL_DestroyWindow(as->window);

    TTF_Quit();
    SDL_Quit();

    SDL_free(as);
}

TTF_Font* load_font(const char *path, int size) {
    TTF_Font *font = TTF_OpenFont(path, size);
    if (!font) {
        SDL_Log("Couldn't open font %s: %s\n", path, SDL_GetError());
    }
    return font;
}