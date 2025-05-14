// paddle.h
#ifndef PADDLE_H
#define PADDLE_H

#include <stdbool.h>
#include <SDL3/SDL.h>
#include <stdlib.h>
#include "score.h" 

/*
 * For future ryan - since struct is declared here, the struct properties are available
 * to anything that includes this header file (public). Consider moving the struct 
 * declaration to the .c file (private) and have getters and setters as needed
 */

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
    float           computer_target_y; // where computer is targetting, to avoid recompute        
    int             reaction_time;     // loops until computer will react to the ball
    const SDL_Color *color;
} Paddle;

// Function declarations
Paddle *paddle_create(
    int             x_pos, 
    int             y_pos,
    int             width,
    int             height,
    float           speed,
    int             player_id,
    bool            is_human,
    const SDL_Color *color
);
void  paddle_free(Paddle *paddle);
void  set_paddle_speed(Paddle *paddle, const float speed);
void  set_computer_target_y(Paddle *paddle, const float y);
float get_computer_target_y(Paddle *paddle);
void  set_reaction_time(Paddle *paddle, const int reaction_time);
int   get_reaction_time(Paddle *paddle);
void  paddle_move(Paddle *paddle);
float get_center_y_coord(const Paddle *paddle);

#endif