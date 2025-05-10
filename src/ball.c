#include <stdlib.h>
#include <stdbool.h>
#include "ball.h"
#include "constants.h"

Ball *create_ball(int x_pos, int y_pos, int width, int height, 
                  float vx, float vy, const SDL_Color *color) {
    Ball *ball = malloc(sizeof(Ball));
    if(ball == NULL)
        return NULL; 
    ball->bounds = (SDL_FRect){ (float)x_pos, (float)y_pos, (float)width, (float)height };
    ball->velocity_x = vx;
    ball->velocity_y = vy;
    ball->color      = color;
    ball->visible    = true;
    return ball;
}

void free_ball(Ball *ball) {
    free(ball);
    ball = NULL;
}

void move_ball(Ball *ball) {
    ball->bounds.x += ball->velocity_x * BALL_SPEED;
    ball->bounds.y += ball->velocity_y * BALL_SPEED;
}