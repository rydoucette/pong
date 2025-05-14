#include <SDL3/SDL.h>
#include <stdio.h>
#include "gamestate.h"
#include "game_logic.h"
#include "render.h"
#include "ball.h"
#include "paddle.h"
#include "app.h"


static void update_play_state(void *appstate) {
    AppState *as = (AppState *)appstate;
    update_ball(as->ball, as->left_paddle, as->right_paddle);
    update_paddles(as->left_paddle,as->right_paddle, as->ball, as->difficulty_level);
    check_and_handle_winner(as);
}

static void render_play_state(void *appstate) {
    AppState *as = (AppState *)appstate;
    draw_background(as->renderer, &COLOR_BG);
    draw_ball(as->renderer, as->ball);
    draw_paddle(as->renderer, as->left_paddle);
    draw_paddle(as->renderer, as->right_paddle);
    draw_scores(as->renderer, 
               as->left_paddle->score,
               as->right_paddle->score,
               as->score_font);
    SDL_RenderPresent(as->renderer);
}

static SDL_AppResult handle_play_event(void *appstate, SDL_Event *event) {
    AppState *as = (AppState *)appstate;

    if (event->type == SDL_EVENT_QUIT) { return SDL_APP_SUCCESS; }

    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
            switch (event->key.scancode) {
                case SDL_SCANCODE_W:
                    if(as->left_paddle->is_human)
                        as->left_paddle->next_dir = DIR_UP;
                    break;
                case SDL_SCANCODE_S:
                    if(as->left_paddle->is_human)
                        as->left_paddle->next_dir = DIR_DOWN;
                    break;
                case SDL_SCANCODE_UP:
                    as->right_paddle->next_dir = DIR_UP;
                    break;
                case SDL_SCANCODE_DOWN:
                    as->right_paddle->next_dir = DIR_DOWN;
                    break;
                default: 
                    break;
            }
            break;
        // Stop moving character when they release the key
        case SDL_EVENT_KEY_UP:
            switch (event->key.scancode) {
                case SDL_SCANCODE_Q:
                case SDL_SCANCODE_ESCAPE:
                    return SDL_APP_SUCCESS;
                case SDL_SCANCODE_P:
                    pause_game(as);
                    break;
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_S:
                    if(as->left_paddle->is_human)
                        as->left_paddle->next_dir = DIR_STOPPED;
                    break;
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_DOWN:
                    as->right_paddle->next_dir = DIR_STOPPED;
                    break;
                default: 
                    break;
            }
            break;
        default: 
            break;
    }
    return SDL_APP_CONTINUE;
}

GameState *create_play_state(void) {
    static GameState state = {
        .update = update_play_state,
        .render = render_play_state,
        .handle_event = handle_play_event,
        .state_id = PLAY
    };
    return &state;
}