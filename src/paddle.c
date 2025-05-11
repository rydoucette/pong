#include <stdio.h>
#include "paddle.h"
#include "constants.h"
#include "app.h" // TODO move positions and colors out of app.h

// Instatiates a paddle struct
Paddle* create_paddle(int x_pos, int y_pos, int width, int height, float speed,
                      int player_id, bool is_human, const SDL_Color *color) {
    Paddle *paddle = malloc(sizeof(Paddle));
    if(paddle == NULL)
        return NULL;
    paddle->bounds = (SDL_FRect){ (float)x_pos, (float)y_pos, (float)width, (float)height };
    paddle->speed       = speed;
    paddle->player_id   = player_id;
    paddle->is_human    = is_human;
    paddle->next_dir    = DIR_STOPPED;
    paddle->color       = color;
    paddle->score       = create_score((player_id == 0) ? (float) SCORE_LEFT_POSITION:  // x pos
                                                          (float) SCORE_RIGHT_POSITION, // x pos
                                                          SCORE_Y_POSITION,     // y pos
                                                          &COLOR_SCORE);        // color
    return paddle;
}

void free_paddle(Paddle *paddle) {
    free(paddle);
    paddle = NULL;
}

void set_paddle_speed(Paddle *paddle, float speed) {
    paddle->speed = speed;
}

// Updates a single paddle's position
void move_paddle(Paddle *paddle) {
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

float get_center_y_coord(Paddle * paddle) {
    return paddle->bounds.y + (paddle->bounds.h/2);
}