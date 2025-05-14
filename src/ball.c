#include <stdlib.h>
#include <stdbool.h>
#include "ball.h"
#include "constants.h"

/*
 * for future ryan,

 * You could have used functions like ball_set_velocity, reverse_velocity_y etc. isntead
 * of having that in game logic.
 * 
 * consider passing float instead of int for positions since they are being cast to float
 * anyways
 * 
 * add const where it makes sense for parameters
*/

Ball *ball_create(int x_pos, int y_pos, int width, int height, float speed,
                  float vx, float vy, const SDL_Color *color) {
    Ball *ball = calloc(1,sizeof(Ball));
    if(ball == NULL)
        return NULL; 
    ball->bounds = (SDL_FRect){ (float)x_pos, (float)y_pos, (float)width, (float)height };
    ball->speed = speed;
    ball->velocity_x = vx;
    ball->velocity_y = vy;
    ball->color      = color;
    ball->visible    = true;
    return ball;
}

void set_ball_speed(Ball *ball, float speed) {
    ball->speed = speed;
}

float get_ball_speed(const Ball *ball) {
    return ball->speed;
}

void ball_free(Ball *ball) {
    free(ball);
}

void ball_move(Ball *ball) {
    ball->bounds.x += ball->velocity_x * get_ball_speed(ball); 
    ball->bounds.y += ball->velocity_y * get_ball_speed(ball);
}