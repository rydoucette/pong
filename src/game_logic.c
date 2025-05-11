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

// computes where the ball will intersect with left or right wall, returns y coordinate
float predict_ball_y_intersect(const Ball *ball, float target_x, float screen_height) {
    float x = ball->bounds.x;
    float y = ball->bounds.y;
    float w  = ball->bounds.h;
    float h  = ball->bounds.h;
    float vx = ball->velocity_x * BALL_SPEED;
    float vy = ball->velocity_y * BALL_SPEED;

    // debug
    //printf("Ball x: %.2f, y: %.2f, w: %.2f, h: %.2f,\n",x,y,w,h);
    //printf("Ball vx: %.2f, vy: %.2f\n",vx,vy);

    // given a balls position and velocity, determine what they will collide with
    bool found = false;
    int iterations_left = 200; // avoid infinite loops until this logic is nailed down
    while(!found && iterations_left >= 0) {
        
        // if upper/lower ball is hit then bounce off the wall
        if(y >= GAME_HEIGHT-1 || y < 0)
            vy *= -1;

        // if ball crosses goal line then stop, the current y is where it will cross
        if((vx > 0 && x >= target_x) || (vx < 0 && x <= target_x)) {
            found = true;
            break;
        }

        // adjust ball position and try again
        x += vx * BALL_SPEED;
        y += vy * BALL_SPEED;

        // safety measure against infinite loops
        iterations_left--;
    }
    //printf("x:%.2f, y:%.2f, vx:%.2f, vy:%.2f\n",iterations_left,x, y, vx, vy);
    return y;
}

// computes a target y coordinate that the paddle should target based on ball trajectory 
void determine_computer_direction(Paddle *paddle, Ball *ball) {
    float paddle__middle_y = (float) get_center_y_coord(paddle); //y coord of center paddle
    float epsilon = 0.5f; // off set the minior difference in floating point accuracy
    float destination_y; // calculated point of where the paddle should go to
    float ball_travel_distance;
    float ball_travel_time;
    float paddle_travel_distance;
    /*
     * If the ball is moving away from the paddle, then reposition paddle to the middle of
     * the screen. Otherwise we need to meet the ball based on its trajectory. 
     */
    if((ball->velocity_x < 0 && paddle->player_id == 1) || 
       (ball->velocity_x > 0 && paddle->player_id == 0)) {
        // ball is moving away from the paddle, reset to the middle of the screen
        destination_y = GAME_HEIGHT / 2.0f;
        //travel_distance = abs(destination_y - paddle__middle_y);
        //travel_time = travel_distance/ball->velocity_x;
        set_paddle_speed(paddle, 0.25f);
    } else {
        // ball is moving towards the paddle, caclulate the ball trajectory
        float paddle_leading_x_coord = (paddle->player_id == 0) ?  1 :  GAME_WIDTH - 1;
        destination_y = predict_ball_y_intersect(ball, 
                                                 paddle_leading_x_coord,
                                                 GAME_HEIGHT);
        ball_travel_distance = fabsf(paddle_leading_x_coord - ball->bounds.x);
        ball_travel_time = ball_travel_distance/fabsf(ball->velocity_x);
        paddle_travel_distance = fabsf(destination_y - paddle__middle_y);
        float paddle_velocity = paddle_travel_distance/ball_travel_time;
        //printf("distance till target: %.2f\n",ball_travel_distance);
        //printf("time till collision: %.2f\n",ball_travel_time);
        set_paddle_speed(paddle,paddle_velocity);
    }
    if(paddle->player_id==1) {
    /*printf("Paddle: %s, Target Y: %.2f, Ball: %.2f\n", 
            (paddle->player_id == 0) ? "paddle-left" : "paddle-right",
             destination_y,
             ball->bounds.y);*/
    }
    if(paddle__middle_y > destination_y + epsilon) {
        paddle->next_dir = DIR_UP;
    } else if(paddle__middle_y < destination_y - epsilon) {
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
    // distance of center of paddle from center of ball
    float relative_intersect_y = (paddle->bounds.y + (paddle->bounds.h / 2.0f)) -
                                 (ball->bounds.y + (ball->bounds.h / 2.0f));
    // scale of -1 to 1 of where the ball hit the paddle
    float normalized = relative_intersect_y / (paddle->bounds.h / 2.0f);

    float vx = -ball->velocity_x; // flip horizontal
    float vy = normalized; // scale of -1 to 1 where the ball hit the paddle

    // Normalize vector
    float magnitude = sqrtf(vx * vx + vy * vy); // magnitude of resulting vector
    ball->velocity_x = (vx / magnitude) * BALL_SPEED; // noramlize it -1 to 1 and scale
    ball->velocity_y = (vy / magnitude) * BALL_SPEED; // noramlize it -1 to 1 and scale
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
                                         (float) PADDLE_SPEED,       // speed
                                         0,                          // player ID
                                         left_is_human,              // is human
                                         &COLOR_P1);                 // color             
    Paddle *right_paddle = create_paddle(p_starting_positions[1][0], // x position
                                         p_starting_positions[1][1], // y position
                                         1,                          // width
                                         TAIL_LENGTH,                // hieght
                                         (float) PADDLE_SPEED,       // speed
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