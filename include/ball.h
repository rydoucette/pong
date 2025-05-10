//ball.hh
#ifndef BALL_H
#define BALL_H

#include <stdbool.h>
#include <SDL3/SDL.h>

// Represents a player or human operated paddle
typedef struct
{
    SDL_FRect       bounds;
    float           velocity_x;
    float           velocity_y;
    bool            visible;
    const SDL_Color *color;
} Ball;

// Function declarations
Ball *create_ball(int x_pos, int y_pos, int width, int height, 
                  float vx, float vy, const SDL_Color *color);
void free_ball(Ball *ball);
void move_ball(Ball *ball);

#endif