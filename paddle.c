#include <stdio.h>
#include "paddle.h"
#include "constants.h"

// Instatiates a paddle struct
Paddle* create_paddle(int x_pos, int y_pos, int width, int height, 
                      int player_id, bool is_human, const SDL_Color *color) {
    Paddle *paddle = malloc(sizeof(Paddle));
    if(paddle == NULL)
        return NULL;
    paddle->bounds = (SDL_FRect){ (float)x_pos, (float)y_pos, (float)width, (float)height };
    paddle->player_id   = player_id;
    paddle->is_human    = is_human;
    paddle->next_dir    = DIR_STOPPED;
    paddle->color       = color;
    return paddle;
}

void free_paddle(Paddle *paddle) {
    free(paddle);
    paddle = NULL;
}

// Updates a single paddle's position
void move_paddle(Paddle *paddle) {
    switch (paddle->next_dir) {
        case DIR_UP:
            paddle->bounds.y = paddle->bounds.y - (1*PADDLE_SPEED);
            break;
        case DIR_DOWN:
            paddle->bounds.y = paddle->bounds.y + (1*PADDLE_SPEED);
            break;
        case DIR_STOPPED:
            break;
    }
}