// game_logic.h
#ifndef GAME_LOGIC_H
#define GANE_LOGIC_H

#include <stdbool.h>
#include "paddle.h"


bool would_collide(Paddle *paddle);
void try_move_paddle(Paddle *paddle);
void update_paddles(Paddle *left_paddle, Paddle *right_paddle);
bool check_goal_scored(Ball *ball);
bool check_wall_collision(Ball *ball);
bool check_paddle_collision(Ball *ball, Paddle *left_paddle, Paddle *right_paddle);
void reflect_ball_from_paddle(Ball *ball);
void reflect_ball_from_wall(Ball *ball);
void reflect_ball(Ball *ball);
void handle_goal(Ball *ball);
void update_ball(Ball *ball, Paddle *left_paddle, Paddle *right_paddle);
void initialize_paddles(void *appstate);
void initialize_ball(void *appstate);
void start_game(void *appstate);

#endif