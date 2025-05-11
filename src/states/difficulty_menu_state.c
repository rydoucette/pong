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

    SDL_Color easy_color_option   = COLOR_OPTION_DISABLED;
    SDL_Color medium_color_option = COLOR_OPTION_DISABLED;
    SDL_Color hard_color_option   = COLOR_OPTION_DISABLED;
    if(as->difficulty_level == EASY)
        easy_color_option = COLOR_OPTION_HIGHLIGHTED;
    else if(as->difficulty_level == MEDIUM)
        medium_color_option = COLOR_OPTION_HIGHLIGHTED;
    else
        hard_color_option = COLOR_OPTION_HIGHLIGHTED;


    draw_message(as->renderer, 
                 "PONG",
                 50,
                 as->title_font,
                 COLOR_TITLE);
    draw_message(as->renderer, 
                 "EASY",
                 (SDL_WINDOW_HEIGHT/2) - 60,
                 as->option_font,
                 easy_color_option);
    draw_message(as->renderer, 
                "MEDIUM",
                (SDL_WINDOW_HEIGHT/2),
                as->option_font,
                medium_color_option);
    draw_message(as->renderer, 
                 "HARD",
                 (SDL_WINDOW_HEIGHT/2) + 60,
                 as->option_font,
                 hard_color_option);
    draw_message(as->renderer, 
                "USE ARROW KEYS TO SCROLL",
                (SDL_WINDOW_HEIGHT/2) + 120,
                as->small_message_font,
                COLOR_MESSAGE_ON);
    draw_message(as->renderer, 
                "PRESS ENTER TO CONFIRM",
                SDL_WINDOW_HEIGHT - 50,
                as->message_font,
                get_flashing_color(COLOR_MESSAGE_ON,COLOR_MESSAGE_OFF));
    
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
                    start_game(as);
                    gamestate_stack_push(as->game_state_stack, create_play_state());
                    break;
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    if(as->difficulty_level > 0)
                        as->difficulty_level--;
                    else
                        as->difficulty_level = 2;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    if(as->difficulty_level < 2)
                        as->difficulty_level++;
                    else
                        as->difficulty_level = 0;
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

GameState *create_difficulty_menu_state(void) {
    static GameState state = {
        .update = update_start_menu_state,
        .render = render_start_menu_state,
        .handle_event = handle_start_menu_event,
        .state_id = START_MENU
    };
    return &state;
}