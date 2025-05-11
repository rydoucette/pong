#include <stdio.h>
#include "paddle.h"
#include "constants.h"

// Instatiates a paddle struct
Paddle* paddle_create(int x_pos, int y_pos, int width, int height, float speed,
                      int player_id, bool is_human, const SDL_Color *color) {
    Paddle *paddle = calloc(1, sizeof(Paddle));
    if(paddle == NULL)
        return NULL;
    paddle->bounds = (SDL_FRect){ (float)x_pos, (float)y_pos, (float)width, (float)height };
    paddle->speed       = speed;
    paddle->player_id   = player_id;
    paddle->is_human    = is_human;
    paddle->next_dir    = DIR_STOPPED;
    paddle->color       = color;
    paddle->score       = score_create(&COLOR_SCORE);
    return paddle;
}

// Free memory allocated for the paddle
void paddle_free(Paddle *paddle) {
    free(paddle->score);
    free(paddle);
}

// Set paddle velocity
void set_paddle_speed(Paddle *paddle, const float speed) {
    paddle->speed = speed;
}

// Move paddle up or down based on velocity
void paddle_move(Paddle *paddle) {
    switch (paddle->next_dir) {
        case DIR_UP:
            paddle->bounds.y = paddle->bounds.y - paddle->speed;
            break;
        case DIR_DOWN:
            paddle->bounds.y = paddle->bounds.y + paddle->speed;
            break;
        case DIR_STOPPED:
            break;
    }
}

// Get y coordinate of the middle of the paddle
float get_center_y_coord(const Paddle * paddle) {
    return paddle->bounds.y + (paddle->bounds.h/2);
}