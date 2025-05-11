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
 * pass ball_speed as argument instead
 * 
 * add const where it makes sense for parameters
*/

Ball *ball_create(int x_pos, int y_pos, int width, int height, 
                  float vx, float vy, const SDL_Color *color) {
    Ball *ball = calloc(1,sizeof(Ball));
    if(ball == NULL)
        return NULL; 
    ball->bounds = (SDL_FRect){ (float)x_pos, (float)y_pos, (float)width, (float)height };
    ball->velocity_x = vx;
    ball->velocity_y = vy;
    ball->color      = color;
    ball->visible    = true;
    return ball;
}

void ball_free(Ball *ball) {
    free(ball);
    ball = NULL;
}

void ball_move(Ball *ball) {
    ball->bounds.x += ball->velocity_x * BALL_SPEED; // todo pass speed as argument
    ball->bounds.y += ball->velocity_y * BALL_SPEED; // todo pass speed as argument
}