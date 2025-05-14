// game_logic.h
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdbool.h>
#include "paddle.h"
#include "ball.h"
#include "app.h"

void handle_game_over(void *appstate, Paddle *winning_paddle, Paddle *losing_paddle);
void check_and_handle_winner(void *appstate);
bool would_collide(Paddle *paddle);
void try_move_paddle(Paddle *paddle);
float predict_ball_y_intersect(const Ball *ball, float target_x, float screen_height);
float get_inaccuracy_level(DifficultyLevel difficulty);
void determine_computer_direction(Paddle *paddle, Ball *ball, DifficultyLevel difficulty);
void update_paddles(Paddle *left_paddle, Paddle *right_paddle, Ball *ball, 
                    DifficultyLevel difficulty);
Paddle *check_goal_scored(Ball *ball, Paddle *left_paddle, Paddle *right_paddle);
bool check_wall_collision(Ball *ball);
Paddle *check_paddle_collision(Ball *ball, Paddle *left_paddle, Paddle *right_paddle);
void reflect_ball_from_paddle(Ball *ball, Paddle *paddle);
void reflect_ball_from_wall(Ball *ball);
void reflect_ball(Ball *ball);
void handle_goal(Ball *ball, Paddle *scoring_paddle);
void update_ball(Ball *ball, Paddle *left_paddle, Paddle *right_paddle);
void initialize_paddles(void *appstate, bool left_is_human, bool right_is_human);
void initialize_ball(void *appstate);
void apply_difficulty_settings(void *appstate);
void pause_game(void *appstate);
void resume_game(void *appstate);
void start_game(void *appstate);

#endif