// paddle.h
#ifndef PADDLE_H
#define PADDLE_H

#include <stdbool.h>
#include <SDL3/SDL.h>
#include <stdlib.h>

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
    int             tail_length;
    int             player_id;
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
    int             player_id, 
    bool            is_human,
    const SDL_Color *color
);
void free_paddle(Paddle *paddle);
void move_paddle(Paddle *paddle);

#endif