#include <SDL3/SDL.h>
#include <stdio.h>
#include "app.h"
#include "gamestate.h"
#include "game_logic.h"
#include "render.h"
#include "ball.h"
#include "paddle.h"

static void update_pause_state(void *appstate) {
    AppState *as = (AppState *)appstate;
}

static void render_pause_state(void *appstate) {
    AppState *as = (AppState *)appstate;
    draw_background(as->renderer, &COLOR_BG);
    draw_ball(as->renderer, as->ball);
    draw_paddle(as->renderer, as->left_paddle);
    draw_paddle(as->renderer, as->right_paddle);
    draw_score(as->renderer, as->left_paddle->score, as->score_font);
    draw_score(as->renderer, as->right_paddle->score, as->score_font);
    draw_message(as->renderer, 
                 "PAUSED",
                 SDL_WINDOW_HEIGHT/2-50,
                 as->title_font,
                 COLOR_STATE);
    draw_message(as->renderer, 
                 "PRESS P TO RESUME",
                 SDL_WINDOW_HEIGHT - 50,
                 as->message_font,
                 get_flashing_color(COLOR_MESSAGE_ON,COLOR_MESSAGE_OFF));
    SDL_RenderPresent(as->renderer);
}

static SDL_AppResult handle_pause_event(void *appstate, SDL_Event *event) {
    AppState *as = (AppState *)appstate;

    if (event->type == SDL_EVENT_QUIT) { return SDL_APP_SUCCESS; }

    switch (event->type) {
        case SDL_EVENT_KEY_UP:
            switch (event->key.scancode) {
                case SDL_SCANCODE_Q:
                case SDL_SCANCODE_ESCAPE:
                    return SDL_APP_SUCCESS;
                case SDL_SCANCODE_P: {
                    resume_game(as);
                    break;
                }
                default: 
                    break;
            }
            break;
        default: 
            break;
    }
    return SDL_APP_CONTINUE;
}

GameState *create_pause_state(void) {
    static GameState state = {
        .update = update_pause_state,
        .render = render_pause_state,
        .handle_event = handle_pause_event,
        .state_id = PAUSE
    };
    return &state;
}