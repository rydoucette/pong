//ball.hh
#ifndef BALL_H
#define BALL_H

#include <stdbool.h>
#include <SDL3/SDL.h>

/*
 * For future ryan - since struct is declared here, the struct properties are available
 * to anything that includes this header file (public). Consider moving the struct 
 * declaration to the .c file (private) and have getters and setters as needed
 */

// Represents a player or human operated paddle
typedef struct
{
    SDL_FRect       bounds;
    float           velocity_x;
    float           velocity_y;
    float           speed;
    bool            visible;
    const SDL_Color *color;
} Ball;

// Function declarations
Ball *ball_create(int x_pos, int y_pos, int width, int height, float speed,
                  float vx, float vy, const SDL_Color *color);
void set_ball_speed(Ball *ball, float speed);
float get_ball_speed(const Ball *ball);
void ball_free(Ball *ball);
void ball_move(Ball *ball);

#endif