// paddle.h
#ifndef PADDLE_H
#define PADDLE_H

#include <stdbool.h>
#include <SDL3/SDL.h>
#include <stdlib.h>
#include "score.h" 

// Possible directions a player can take
typedef enum
{
    DIR_UP,
    DIR_DOWN,
    DIR_STOPPED
} PaddleDirection;


// Represents a player or human operated paddle
typedef struct
{
    SDL_FRect       bounds;
    float           speed;
    int             player_id;
    Score           *score;
    bool            is_human;
    PaddleDirection next_dir;
    const SDL_Color *color;
} Paddle;

// Function declarations
Paddle *create_paddle(
    int             x_pos, 
    int             y_pos,
    int             width,
    int             height,
    float           speed,
    int             player_id,
    bool            is_human,
    const SDL_Color *color
);
void  free_paddle(Paddle *paddle);
void  set_paddle_speed(Paddle *paddle, float speed);
void  move_paddle(Paddle *paddle);
float get_center_y_coord(Paddle *paddle);

#endif