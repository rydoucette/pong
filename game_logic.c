#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "constants.h"
#include "ball.h"
#include "app.h"

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

// Move paddle if unobstructed
void update_paddles(Paddle *left_paddle, Paddle *right_paddle) {
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
    // Optionally, modify vertical velocity based on paddle position
    float relative_intersect_y = (paddle->bounds.y + paddle->bounds.h / 2.0f) - (ball->bounds.y + ball->bounds.h / 2.0f);
    float normalized = relative_intersect_y / (paddle->bounds.h / 2.0f);
    ball->velocity_y = normalized * BALL_SPEED;  // Customize for desired effect
    ball->velocity_x *= -1; 
}

// Ball bounces off upper or lower wall
void reflect_ball_from_wall(Ball *ball) {
    ball->velocity_y *= -1; 
}

// Reset ball position, update player's score
void handle_goal(Ball *ball, Paddle *scoring_paddle) {
    ball->bounds.x = GAME_WIDTH/2;
    ball->bounds.y = GAME_HEIGHT/2;
}

void update_ball(Ball *ball, Paddle *left_paddle, Paddle *right_paddle) {
    Paddle *colliding_paddle      = check_paddle_collision(ball,left_paddle,right_paddle);
    bool collided_with_upper_wall = check_wall_collision(ball);
    Paddle *scoring_paddle        = check_goal_scored(ball,left_paddle,right_paddle);

    if(colliding_paddle) {
        reflect_ball_from_paddle(ball,colliding_paddle);
    } else if(collided_with_upper_wall) {
        reflect_ball_from_wall(ball);
    } else if(scoring_paddle) {
        handle_goal(ball,scoring_paddle);
    }
    move_ball(ball);
} 

// Create left and right game paddles
void initialize_paddles(void *appstate) {
    AppState *as = (AppState *)appstate;
    Paddle *left_paddle  = create_paddle(p_starting_positions[0][0], // x position
                                         p_starting_positions[0][1], // y position
                                         1,                          // width
                                         TAIL_LENGTH,                // height
                                         0,                          // player ID
                                         true,                       // is human
                                         &COLOR_P1);                 // paddle color             
    Paddle *right_paddle = create_paddle(p_starting_positions[1][0], // x position
                                         p_starting_positions[1][1], // y position
                                         1,                          // width
                                         TAIL_LENGTH,                // hieght
                                         1,                          // player ID
                                         false,                      // is human
                                         &COLOR_P2);                 // paddle color   
    as->left_paddle  = left_paddle;
    as->right_paddle = right_paddle;
}

// Create the game ball
void initialize_ball(void *appstate) {
    AppState *as = (AppState *)appstate;
    Ball *ball = create_ball(GAME_WIDTH/2,                       // x position
                             GAME_HEIGHT/2,                      // y position
                             1,                                  // width
                             1,                                  // height
                             0.5,                                  // velocity x
                             0,                                  // velocity y
                             &COLOR_BALL);                       // color
    as->ball = ball;
}

// Kick off the core game cycle
void start_game(void *appstate) {
    AppState *as = (AppState *)appstate;

    as->state      = RUNNING;
    as->pause_time = SDL_GetTicks();
    as->last_step  = SDL_GetTicks();
    as->game_mode  = PVP;

    initialize_paddles(as);
    initialize_ball(as);
    //initialize_score(as);
}