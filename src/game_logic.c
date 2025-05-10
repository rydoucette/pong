#include <SDL3/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "app.h"
#include "constants.h"
#include "game_logic.h"
#include "ball.h"
#include "score.h"

// game over logic
void handle_game_over(void *appstate, Paddle *winning_paddle, Paddle *losing_paddle) {
    AppState *as = (AppState *)appstate;
    as->ball->visible = false;
    winning_paddle->color = &COLOR_WINNER;
    losing_paddle->color  = &COLOR_LOSER;
    winning_paddle->score->color = &COLOR_WINNER;
    losing_paddle->score->color  = &COLOR_LOSER;
    gamestate_stack_push(as->game_state_stack, create_game_over_state());
}

// Checks if any player reaches desired score, and if so makes a call to handle game over
void check_and_handle_winner(void *appstate) {
    AppState *as = (AppState *)appstate;
    Paddle *winner = NULL;
    Paddle *loser  = NULL;

    if(as->left_paddle->score->score == WINNING_SCORE) {
        winner = as->left_paddle;
        loser  = as->right_paddle;
    } else if(as->right_paddle->score->score == WINNING_SCORE) {
        winner = as->right_paddle;
        loser  = as->left_paddle;
    }

    if(winner && loser) {
        handle_game_over(as, winner, loser);
    }

}

// Compares paddle position to upper and lower bounds
bool would_collide(Paddle *paddle) {
    switch(paddle->next_dir) {
        case DIR_DOWN:
            if(paddle->bounds.y + paddle->bounds.h >= GAME_HEIGHT )
                return true;
        break;
        case DIR_UP:
            if(paddle->bounds.y <= 0) 
                return true;
        break;
        case DIR_STOPPED:
            //do nothing
        break;
    }
    return false;
}

// Will not move paddle if it collides with upper or lower wall
void try_move_paddle(Paddle *paddle) {
    if(!would_collide(paddle))
        move_paddle(paddle);
}

float predict_ball_y_intersect(const Ball *ball, float target_x, float screen_height) {
    float x = ball->bounds.x;
    float y = ball->bounds.y;
    float vx = ball->velocity_x * BALL_SPEED;
    float vy = ball->velocity_y * BALL_SPEED;
    float h  = ball->bounds.h;

    // Bail early if vx is zero (no horizontal movement)
    if (vx == 0) return y + h / 2.0f;

    // Clamp simulation steps to avoid infinite loops
    int max_iterations = 1000;
    while (((vx > 0 && x < target_x) || (vx < 0 && x > target_x)) && max_iterations-- > 0) {
        float time_to_x = (target_x - x) / vx;

        // Avoid division by zero
        float time_to_wall = INFINITY;
        if (vy != 0) {
            time_to_wall = (vy > 0)
                ? (screen_height - h - y) / vy
                : -y / vy;
        }

        float dt = fminf(time_to_x, time_to_wall);

        x += vx * dt;
        y += vy * dt;

        // Handle bounce and clamp inside bounds
        if (y <= 0) {
            y = -y;
            vy = -vy;
        } else if (y + h >= screen_height) {
            y = 2 * (screen_height - h) - y;
            vy = -vy;
        }
    }

    return y + h / 2.0f;
}

void determine_computer_direction(Paddle *paddle, Ball *ball) {
    float paddle_y = (float) get_center_y_coord(paddle);
    float epsilon = 0.5f;
    float target;
    float target_center = GAME_HEIGHT / 2.0f;

    /*
     * If the ball is moving away from the paddle, then reposition paddle to the middle of
     * the screen. Other we need to meet the ball based on its trajectory. 
     */
    if((ball->velocity_x < 0 && paddle->player_id == 1) || 
       (ball->velocity_x > 0 && paddle->player_id == 0)) {
        target = GAME_HEIGHT / 2.0f;
    } else {
        float paddle_x = (paddle->player_id == 0) ? GAME_WIDTH - BLOCK_SIZE_IN_PIXELS
                                                  : BLOCK_SIZE_IN_PIXELS;
        target = predict_ball_y_intersect(ball, paddle_x, GAME_HEIGHT);
    }
    printf("Target Y: %.2f, Ball: %.2f, Paddle Y: %.2f\n", target, ball->bounds.y,paddle_y);
    if(paddle_y > target + epsilon) {
        paddle->next_dir = DIR_UP;
    } else if(paddle_y < target - epsilon) {
        paddle->next_dir = DIR_DOWN;
    } else {
        paddle->next_dir = DIR_STOPPED;
    }
}

// Move paddle if unobstructed
void update_paddles(Paddle *left_paddle, Paddle *right_paddle, Ball *ball) {
    if(!left_paddle->is_human)
        determine_computer_direction(left_paddle, ball);
    if(!right_paddle->is_human)
        determine_computer_direction(right_paddle, ball);
    try_move_paddle(left_paddle);
    try_move_paddle(right_paddle);
}

// Checks if a goal was scored, and returns the paddle that scored
Paddle *check_goal_scored(Ball *ball, Paddle *left_paddle, Paddle *right_paddle) {
    if(ball->bounds.x < 0)
        return right_paddle;
    else if(ball->bounds.x > GAME_WIDTH)
        return left_paddle;
    else
        return NULL;
}

// Detect ball collision with upper or lower wall
bool check_wall_collision(Ball *ball) {
    return ball->bounds.y >= GAME_HEIGHT-1 || ball->bounds.y < 0;
}

// Detect if ball collided with a paddle, returns the colliding paddle
Paddle *check_paddle_collision(Ball *ball, Paddle *left_paddle, Paddle *right_paddle) {
    if(SDL_HasRectIntersectionFloat(&ball->bounds, &left_paddle->bounds))
        return left_paddle;
    else if(SDL_HasRectIntersectionFloat(&ball->bounds, &right_paddle->bounds))
        return right_paddle;
    else
        return NULL;
}

// Ball bounces off a paddle
void reflect_ball_from_paddle(Ball *ball, Paddle *paddle) {
    float relative_intersect_y = (paddle->bounds.y + paddle->bounds.h / 2.0f) -
                                 (ball->bounds.y + ball->bounds.h / 2.0f);
    float normalized = relative_intersect_y / (paddle->bounds.h / 2.0f);

    float vx = -ball->velocity_x; // flip horizontal
    float vy = normalized;

    // Normalize vector
    float magnitude = sqrtf(vx * vx + vy * vy);
    ball->velocity_x = (vx / magnitude) * BALL_SPEED;
    ball->velocity_y = (vy / magnitude) * BALL_SPEED;
}

// Ball bounces off upper or lower wall
void reflect_ball_from_wall(Ball *ball) {
    ball->velocity_y *= -1; 
}

// Reset ball position, update player's score and serve to the winner
void handle_goal(Ball *ball, Paddle *scoring_paddle) {
    ball->bounds.y = GAME_HEIGHT/2;
    ball->velocity_y = 0;

    if(scoring_paddle->player_id == 0) {
        // serve to the left
        ball->bounds.x = 3*GAME_WIDTH/4;
        ball->velocity_x = -0.5;
        increase_score(scoring_paddle->score);
    } else {
        // serve to the right
        ball->velocity_x = 0.5;
        ball->bounds.x = GAME_WIDTH/4;
        increase_score(scoring_paddle->score);
    }

}

void update_ball(Ball *ball, Paddle *left_paddle, Paddle *right_paddle) {
    Paddle *colliding_paddle      = check_paddle_collision(ball,left_paddle,right_paddle);
    bool collided_with_upper_wall = check_wall_collision(ball);
    Paddle *scoring_paddle        = check_goal_scored(ball,left_paddle,right_paddle);
    if(colliding_paddle) {
        // had an issue with ball infintely bouncing inside a paddle, to fix this the ball 
        // will only bounce if colliding with the paddle it is moving towards
        if(ball->velocity_x >  0 && colliding_paddle->player_id == 1 || 
           ball->velocity_x <= 0 && colliding_paddle->player_id == 0) 
            reflect_ball_from_paddle(ball,colliding_paddle);
    } else if(collided_with_upper_wall) {
        reflect_ball_from_wall(ball);
    } else if(scoring_paddle) {
        handle_goal(ball,scoring_paddle);
    }
    move_ball(ball);
} 

// Create left and right game paddles
void initialize_paddles(void *appstate, bool left_is_human, bool right_is_human) {
    AppState *as = (AppState *)appstate;
    // if pvp then left_paddle should be human otherwise false 
    Paddle *left_paddle  = create_paddle(p_starting_positions[0][0], // x position
                                         p_starting_positions[0][1], // y position
                                         1,                          // width
                                         TAIL_LENGTH,                // height
                                         0,                          // player ID
                                         left_is_human,              // is human
                                         &COLOR_P1);                 // color             
    Paddle *right_paddle = create_paddle(p_starting_positions[1][0], // x position
                                         p_starting_positions[1][1], // y position
                                         1,                          // width
                                         TAIL_LENGTH,                // hieght
                                         1,                          // player ID
                                         right_is_human,             // is human
                                         &COLOR_P2);                 // color   
    as->left_paddle  = left_paddle;
    as->right_paddle = right_paddle;
}

// Create the game ball
void initialize_ball(void *appstate) {
    AppState *as = (AppState *)appstate;
    Ball *ball = create_ball(GAME_WIDTH/2,  // x position
                             GAME_HEIGHT/2, // y position
                             1,             // width
                             1,             // height
                             0.5,           // velocity x
                             0,             // velocity y
                             &COLOR_BALL);  // color
    as->ball = ball;
}

// Resume the game
void resume_game(void *appstate) {
    AppState *as = (AppState *)appstate;
    GameState *current;
    gamestate_stack_pop(as->game_state_stack, &current);
    as->last_step = SDL_GetTicks();
}

// Pause the game
void pause_game(void *appstate) {
    AppState *as = (AppState *)appstate;
    gamestate_stack_push(as->game_state_stack, create_pause_state());
}

// Kick off the core game cycle
void start_game(void *appstate) {
    AppState *as = (AppState *)appstate;
    as->last_step  = SDL_GetTicks();
    initialize_paddles(as,(as->game_mode==PVP) ? true : false, true);
    initialize_ball(as);
    gamestate_stack_push(as->game_state_stack, create_play_state());
}