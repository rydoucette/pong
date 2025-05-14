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
        paddle_move(paddle);
}

// computes where the ball will intersect with left or right wall, returns y coordinate
float predict_ball_y_intersect(const Ball *ball, float target_x, float screen_height) {
    float x = ball->bounds.x;
    float y = ball->bounds.y;
    float w  = ball->bounds.h;
    float h  = ball->bounds.h;
    float vx = ball->velocity_x * get_ball_speed(ball);
    float vy = ball->velocity_y * get_ball_speed(ball);

    printf("get ball speed from: %f\n",get_ball_speed(ball));

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
        x += vx * get_ball_speed(ball);
        y += vy * get_ball_speed(ball);

        // safety measure against infinite loops
        iterations_left--;
    }
    //printf("x:%.2f, y:%.2f, vx:%.2f, vy:%.2f\n",iterations_left,x, y, vx, vy);
    return y;
}

float get_inaccuracy_level(DifficultyLevel difficulty) {
    float offset = 0;
    switch(difficulty) {
        case EASY:
            offset =  SDL_randf() + (float) INACCURACY_EASY;
            break;
        case MEDIUM:
            offset = SDL_randf() + (float) INACCURACY_HARD;
            break;
        case HARD:
            offset = SDL_randf() + (float) INACCURACY_HARD;
            break;
        case DEMO:
            offset = (float) 0;
            break;
        default:
            offset = (float) 0;
            break;
    }
    if(SDL_rand(1)%2 == 0)
        return offset * -1;
    else
        return offset;
}

// computes a target y coordinate that the paddle should target based on ball trajectory 
void determine_computer_direction(Paddle *paddle, Ball *ball, DifficultyLevel difficulty) {
    /*
     * add a reaction delay
     * add inaccuracy
     * capp the ai paddle speed
     * make ai ignore some balls
     * 
     */
    float paddle__middle_y = (float) get_center_y_coord(paddle); //y coord of center paddle
    float epsilon = PADDLE_MOVE_EPSILON; // offset minor float accuracy
    float destination_y; // calculated point of where the paddle should go to
    float ball_travel_distance;
    float ball_travel_time;
    float paddle_travel_distance;
    float inaccuracy = get_inaccuracy_level(difficulty);
    /*
     * If the ball is moving away from the paddle, then reposition paddle to the middle of
     * the screen. Otherwise we need to meet the ball based on its trajectory. 
     */
    if((ball->velocity_x < 0 && paddle->player_id == PADDLE_RIGHT) || 
       (ball->velocity_x > 0 && paddle->player_id == PADDLE_LEFT)) {
        // ball is moving away from the paddle, reset to the middle of the screen
        
        if(difficulty == DEMO) {
            set_paddle_speed(paddle, (float) RETURN_TO_CENTER_SPEED);
            destination_y = GAME_HEIGHT / 2.0f;
        } else if(difficulty == HARD) {
            destination_y = GAME_HEIGHT / 2.0f;
        } else {
            destination_y = paddle__middle_y;
        }

    } else {
        // ball is moving towards the paddle, caclulate the ball trajectory
        float paddle_leading_x_coord = (paddle->player_id == PADDLE_LEFT)? 1: GAME_WIDTH-1;
        destination_y = predict_ball_y_intersect(ball, 
                                                 paddle_leading_x_coord,
                                                 GAME_HEIGHT);
        if(difficulty == DEMO) {
            ball_travel_distance = fabsf(paddle_leading_x_coord - ball->bounds.x);
            ball_travel_time = ball_travel_distance/fabsf(ball->velocity_x);
            paddle_travel_distance = fabsf(destination_y - paddle__middle_y);
            float paddle_velocity = paddle_travel_distance/ball_travel_time;
            set_paddle_speed(paddle,paddle_velocity);
        }
    }

    // compare paddle position relative to the computed target, move in that direction 
    if(paddle__middle_y > destination_y + epsilon) {
        paddle->next_dir = DIR_UP;
    } else if(paddle__middle_y < destination_y - epsilon) {
        paddle->next_dir = DIR_DOWN;
    } else {
        paddle->next_dir = DIR_STOPPED;
    }
    paddle->computer_target_y = destination_y;
}

// Move paddle if unobstructed
void update_paddles(Paddle *left_paddle, Paddle *right_paddle, Ball *ball, 
                    DifficultyLevel difficulty) {
    if(difficulty == DEMO) {
        determine_computer_direction(left_paddle, ball, difficulty); 
        determine_computer_direction(right_paddle, ball, difficulty); 
    } else {
        if(!left_paddle->is_human) {
            if(left_paddle->reaction_time <= 0) {
                determine_computer_direction(left_paddle, ball, difficulty); 
                reset_reaction_time(left_paddle);
            } else {
                left_paddle->reaction_time--;
            }
        }
    
        if(!right_paddle->is_human) {
            if(right_paddle->reaction_time <= 0) {
                determine_computer_direction(right_paddle, ball, difficulty); 
                reset_reaction_time(right_paddle);
            } else {
                right_paddle->reaction_time--;
            }
        }
    }
                    
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
    ball->velocity_x = (vx / magnitude) * get_ball_speed(ball);
    ball->velocity_y = (vy / magnitude) * get_ball_speed(ball); 
}

// Ball bounces off upper or lower wall
void reflect_ball_from_wall(Ball *ball) {
    ball->velocity_y *= -1; 
}

// Reset ball position, update player's score and serve to the winner
void handle_goal(Ball *ball, Paddle *scoring_paddle) {
    if(scoring_paddle->player_id == PADDLE_LEFT) {
        // serve to the left
        ball->bounds.x =   (float) SERVE_TO_LEFT_X_POS;
        ball->bounds.y =   (float) SERVE_TO_LEFT_Y_POS;
        ball->velocity_x = (float) BALL_SERVE_VX * -1;
        ball->velocity_y = (float) BALL_SERVE_VY;
    } else {
        // serve to the right
        ball->bounds.x =   (float) SERVE_TO_RIGHT_X_POS;
        ball->bounds.y =   (float) SERVE_TO_RIGHT_Y_POS;
        ball->velocity_x = (float) BALL_SERVE_VX;
        ball->velocity_y = (float) BALL_SERVE_VY;
    }
    increase_score(scoring_paddle->score);
}

// Update ball position and velocity taking into account possible collisions and goals
void update_ball(Ball *ball, Paddle *left_paddle, Paddle *right_paddle) {
    Paddle *colliding_paddle      = check_paddle_collision(ball,left_paddle,right_paddle);
    bool collided_with_upper_wall = check_wall_collision(ball);
    Paddle *scoring_paddle        = check_goal_scored(ball,left_paddle,right_paddle);
    if(colliding_paddle) {
        // had an issue with ball infintely bouncing inside a paddle, to fix this the ball 
        // will only bounce if colliding with the paddle it is moving towards
        if(ball->velocity_x >  0 && colliding_paddle->player_id == PADDLE_RIGHT || 
           ball->velocity_x <= 0 && colliding_paddle->player_id == PADDLE_LEFT) 
            reflect_ball_from_paddle(ball,colliding_paddle);
        left_paddle->computer_target_y = -1;  // Reset targets for computers whenever ball is hit
        right_paddle->computer_target_y = -1; // Reset targets for computers whenever ball is hit
    } else if(collided_with_upper_wall) {
        reflect_ball_from_wall(ball);
    } else if(scoring_paddle) {
        handle_goal(ball,scoring_paddle);
        left_paddle->computer_target_y = -1;  // Reset targets for computers whenever ball is hit
        right_paddle->computer_target_y = -1; // Reset targets for computers whenever ball is hit
    }
    ball_move(ball);
} 

// Create left and right game paddles
void initialize_paddles(void *appstate, bool left_is_human, bool right_is_human) {
    AppState *as = (AppState *)appstate;

    int computer_reaction_delay = as->difficulty_settings->ai_reaction_time;
    float left_paddle_speed = (left_is_human)  ? PADDLE_SPEED : as->difficulty_settings->paddle_speed;
    float right_paddle_speed = (right_is_human) ? PADDLE_SPEED : as->difficulty_settings->paddle_speed;
    int reaction_time = as->difficulty_settings->ai_reaction_time;
 
    Paddle *left_paddle  = paddle_create(LEFT_PADDLE_START_X,        // x position
                                         PADDLE_START_Y,             // y position
                                         PADDLE_WIDTH,               // width
                                         TAIL_LENGTH,                // height
                                         (float) left_paddle_speed,  // speed
                                         0,                          // player ID
                                         left_is_human,              // is human
                                         reaction_time,              // ai reaction time
                                         &COLOR_P1);                 // color             
    Paddle *right_paddle = paddle_create(RIGHT_PADDLE_START_X,       // x position
                                         PADDLE_START_Y,             // y position
                                         PADDLE_WIDTH,               // width
                                         TAIL_LENGTH,                // hieght
                                         (float) right_paddle_speed, // speed
                                         1,                          // player ID
                                         right_is_human,             // is human
                                         reaction_time,              // ai reaction time
                                         &COLOR_P2);                 // color   
              
    as->left_paddle  = left_paddle;
    as->right_paddle = right_paddle;
}

// Create the game ball
void initialize_ball(void *appstate) {
    AppState *as = (AppState *)appstate;
    Ball *ball = ball_create(SERVE_TO_RIGHT_X_POS,                // x position
                             SERVE_TO_RIGHT_Y_POS,                // y position
                             BALL_BLOCK_SIZE,                     // width
                             BALL_BLOCK_SIZE,                     // height
                             as->difficulty_settings->ball_speed, // ball speed
                             BALL_SERVE_VX,                       // velocity x
                             BALL_SERVE_VY,                       // velocity y

                             &COLOR_BALL);                        // color
    as->ball = ball;
}

// Apply difficulty specific settings
void apply_difficulty_settings(void *appstate) {
    AppState *as = (AppState *)appstate;
    switch(as->difficulty_level) {
        case EASY:
            as->difficulty_settings = &EASY_SETTINGS;
            break;
        case MEDIUM:
            as->difficulty_settings = &MEDIUM_SETTINGS;
            break;
        case HARD:
            as->difficulty_settings = &HARD_SETTINGS;
            break;
        case DEMO:
            as->difficulty_settings = &DEMO_SETTINGS;
            break;
        default:
            as->difficulty_settings = &EASY_SETTINGS;
            break;
    }
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