#include <SDL3/SDL.h>
#include <stdio.h>
#include "gamestate.h"
#include "game_logic.h"
#include "render.h"
#include "ball.h"
#include "paddle.h"
#include "app.h"


static void update_start_menu_state(void *appstate) {
    AppState *as = (AppState *)appstate;
    update_ball(as->ball, as->left_paddle, as->right_paddle);
    update_paddles(as->left_paddle,as->right_paddle, as->ball);
}

static void render_start_menu_state(void *appstate) {
    AppState *as = (AppState *)appstate;
    draw_background(as->renderer, &COLOR_BG);
    draw_start_menu(as->renderer,&COLOR_START_MENU);

    SDL_Color pvp_color_option = COLOR_OPTION_DISABLED;
    SDL_Color pve_color_option = COLOR_OPTION_DISABLED;
    if(as->game_mode == PVP)
        pvp_color_option = COLOR_OPTION_HIGHLIGHTED;
    else
        pve_color_option = COLOR_OPTION_HIGHLIGHTED;

    draw_message(as->renderer, 
                 "PONG",
                 50,
                 as->title_font,
                 COLOR_TITLE);
    draw_message(as->renderer, 
                 "PRESS ENTER TO CONFIRM",
                 SDL_WINDOW_HEIGHT - 50,
                 as->message_font,
                 get_flashing_color(COLOR_MESSAGE_ON,COLOR_MESSAGE_OFF));
    draw_message(as->renderer, 
                 "USE ARROW KEYS TO SCROLL",
                 (SDL_WINDOW_HEIGHT/2) + 120,
                 as->small_message_font,
                 COLOR_MESSAGE_ON);
    draw_message(as->renderer, 
                 "VS PLAYER",
                 (SDL_WINDOW_HEIGHT/2) - 40,
                 as->option_font,
                 pvp_color_option);
    draw_message(as->renderer, 
                 "VS COMPUTER",
                 (SDL_WINDOW_HEIGHT/2) + 40,
                 as->option_font,
                 pve_color_option);
    
    draw_ball(as->renderer, as->ball);
    draw_paddle(as->renderer, as->left_paddle);
    draw_paddle(as->renderer, as->right_paddle);

    SDL_RenderPresent(as->renderer);
}

static SDL_AppResult handle_start_menu_event(void *appstate, SDL_Event *event) {
    AppState *as = (AppState *)appstate;

    if (event->type == SDL_EVENT_QUIT) { return SDL_APP_SUCCESS; }

    switch (event->type) {
        case SDL_EVENT_KEY_UP:
            switch (event->key.scancode) {
                case SDL_SCANCODE_Q:
                case SDL_SCANCODE_ESCAPE:
                    return SDL_APP_SUCCESS;
                    break;
                case SDL_SCANCODE_KP_ENTER:
                case SDL_SCANCODE_RETURN:
                    // lock in choice
                    // start game
                    if(as->game_mode == PVP) {
                        start_game(as);
                        gamestate_stack_push(as->game_state_stack, create_play_state());
                    } else {
                        gamestate_stack_push(as->game_state_stack, create_difficulty_menu_state());
                    }
                    break;
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    as->game_mode ^= 1U; // toggle game_mode
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

GameState *create_start_menu_state(void) {
    static GameState state = {
        .update = update_start_menu_state,
        .render = render_start_menu_state,
        .handle_event = handle_start_menu_event,
        .state_id = START_MENU
    };
    return &state;
}