#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>
#include "constants.h"
#include "app.h"
#include "paddle.h"
#include "game_logic.h"
#include "render.h"
#include "ball.h"

/*
TODO: 
   learn more about the compilation and build process:
      extern vs static const
      linking step
      cmakelistlist add executables
    Better way for getting and handling key events and passing them to objects?
    learn about interpolation
*/

static SDL_AppResult handle_key_up_event(void *appstate, SDL_Scancode key_code) {
    // stop player movement on key up
    AppState *as = (AppState *)appstate;
    Paddle *paddle = NULL;

    // find out which player's key was pressed if any
    if(key_code == player_keys[0][0] || key_code == player_keys[0][1])
        paddle = as->left_paddle;
    if(key_code == player_keys[1][0] || key_code == player_keys[1][1])
        paddle = as->right_paddle;

    switch (key_code) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_W:
            if(as->state == RUNNING && paddle) {
                paddle->next_dir = DIR_STOPPED;
            }
        break;
        case SDL_SCANCODE_DOWN:
        case SDL_SCANCODE_S:
            if(as->state == RUNNING && paddle) {
                paddle->next_dir = DIR_STOPPED;
            }
            break;
        default:
        break;
    }
    return SDL_APP_CONTINUE;
}

// map key presses to specific characters and game controls such as pause, reset, enter etc.
static SDL_AppResult handle_key_down_event(void *appstate, SDL_Scancode key_code) {
    AppState *as = (AppState *)appstate;
    Paddle *paddle = NULL;

    // find out which player's key was pressed if any
    if(key_code == player_keys[0][0] || key_code == player_keys[0][1])
        paddle = as->left_paddle;
    if(key_code == player_keys[1][0] || key_code == player_keys[1][1])
        paddle = as->right_paddle;

    switch (key_code) {
    /* Quit. */
    case SDL_SCANCODE_ESCAPE:
    case SDL_SCANCODE_Q:
        return SDL_APP_SUCCESS;
    /* Restart the game as if the program was launched. */
    case SDL_SCANCODE_R:
    case SDL_SCANCODE_SPACE:
        start_game(as);
        break;
    /* Decide new direction of the character. */
    case SDL_SCANCODE_UP:
    case SDL_SCANCODE_W:
        if(as->state == RUNNING && paddle) {
            paddle->next_dir = DIR_UP;
        }
        break;
    case SDL_SCANCODE_DOWN:
    case SDL_SCANCODE_S:
        if(as->state == RUNNING && paddle) {
            paddle->next_dir = DIR_DOWN;
        }
        break;
    /* Pause the game. */
    case SDL_SCANCODE_P:
        //toggle_pause(as);
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

// Direct the game logic to game_logic.c
void update_game(void *appstate) {
    AppState *as = (AppState *)appstate;
    update_paddles(as->left_paddle,as->right_paddle);
    update_ball(as->ball, as->left_paddle, as->right_paddle);
}

// Direct rendering to screen to render.c
void render_game(void *appstate) {
    AppState *as = (AppState *)appstate;
    draw_background(as->renderer,&COLOR_BG);
    draw_ball(as->renderer, as->ball);
    draw_paddle(as->renderer, as->left_paddle);
    draw_paddle(as->renderer, as->right_paddle);
    //draw_score(as->p1_score);
    //draw_score(as->p2_score);
    SDL_RenderPresent(as->renderer);
}

// This function runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState *as = (AppState *)appstate;
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        return handle_key_down_event(as, event->key.scancode);
    case SDL_EVENT_KEY_UP:
        return handle_key_up_event(as, event->key.scancode);
    default:
        break;
    }
    return SDL_APP_CONTINUE;
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

    start_game(as);

    return SDL_APP_CONTINUE;
}

// This function runs once per frame, and is the heart of the program
SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *as = (AppState *)appstate;
    const Uint64 now = SDL_GetTicks();

    while(now - as->last_step >= STEP_RATE_IN_MILLISECONDS) {
        update_game(as); 
        as->last_step += STEP_RATE_IN_MILLISECONDS;
    }
    render_game(as);

    return SDL_APP_CONTINUE;
}

// This function runs once at shutdown
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (appstate != NULL) {
        AppState *as = (AppState *)appstate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        free_paddle(as->left_paddle);
        free_paddle(as->right_paddle);
        free_ball(as->ball);
        SDL_free(as);
    }

}