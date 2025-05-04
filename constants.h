// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define GAME_WIDTH                80U
#define GAME_HEIGHT               50U
#define BLOCK_SIZE_IN_PIXELS      16
#define SDL_WINDOW_WIDTH          (BLOCK_SIZE_IN_PIXELS * GAME_WIDTH)
#define SDL_WINDOW_HEIGHT         (BLOCK_SIZE_IN_PIXELS * GAME_HEIGHT)
#define PLAYER_COUNT              2
#define TAIL_LENGTH               8
#define PADDLE_SPEED              0.5
#define BALL_SPEED                1
#define STEP_RATE_IN_MILLISECONDS 15
#define DEFAULT_VOLUME            0.5

#endif