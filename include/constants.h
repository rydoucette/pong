// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

// Game window and sizes
#define GAME_WIDTH                60U
#define GAME_HEIGHT               40U
#define BLOCK_SIZE_IN_PIXELS      16
#define SDL_WINDOW_WIDTH          (BLOCK_SIZE_IN_PIXELS * GAME_WIDTH)
#define SDL_WINDOW_HEIGHT         (BLOCK_SIZE_IN_PIXELS * GAME_HEIGHT)
#define BALL_BLOCK_SIZE           1

// Positioning
#define LEFT_PADDLE_START_X      0
#define RIGHT_PADDLE_START_X     (GAME_WIDTH - 1)
#define PADDLE_START_Y           (GAME_HEIGHT / 3)
#define SCORE_LEFT_POSITION       0.43
#define SCORE_RIGHT_POSITION      0.57
#define SCORE_Y_POSITION          20

// Fonts
#define FONT_PATH                 "assets/fonts/Audiowide-Regular.ttf"
#define FONT_SIZE_TITLE           64
#define FONT_SIZE_OPTION          32
#define FONT_SIZE_MESSAGE         24
#define FONT_SIZE_SMALL_MESSAGE   16
#define FONT_SIZE_SCORE           64

// Game logic variables
#define PLAYER_COUNT              2
#define PADDLE_LEFT               0
#define PADDLE_RIGHT              1
#define WINNING_SCORE             5
#define BALL_SERVE_VX             0.5
#define BALL_SERVE_VY             0.25
#define SERVE_TO_LEFT_X_POS       (0.6 * GAME_WIDTH)
#define SERVE_TO_RIGHT_X_POS      (0.4 * GAME_WIDTH)
#define SERVE_TO_LEFT_Y_POS       (0.10 * GAME_HEIGHT)
#define SERVE_TO_RIGHT_Y_POS      (0.10 * GAME_HEIGHT)
#define PADDLE_MOVE_EPSILON       0.5
#define GAME_STATE_STACK_SIZE     4
#define TAIL_LENGTH               8
#define PADDLE_WIDTH              1
#define FLASH_INTERVAL_MS         250

// Speeds
#define STEP_RATE_IN_MILLISECONDS 15
#define PADDLE_SPEED              0.66
#define RETURN_TO_CENTER_SPEED    0.25
#define BALL_SPEED                1
#define REFLECTION_BOOST          1.5

// Background
static const SDL_Color COLOR_BG                   = {17, 17, 17, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_START_MENU           = {35, 30, 60, SDL_ALPHA_OPAQUE};

// Ball and paddles
static const SDL_Color COLOR_P1                   = {0, 200, 255, SDL_ALPHA_OPAQUE};   
static const SDL_Color COLOR_P2                   = {255, 100, 100, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_BALL                 = {190, 170, 160, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_WINNER               = {255, 215, 0, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_LOSER                = {105, 105, 105, SDL_ALPHA_OPAQUE};

// Font colors
static const SDL_Color COLOR_SCORE                = {250, 250, 250, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_TITLE                = {255, 255, 255, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_STATE                = {250, 250, 250, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_MESSAGE_OFF          = {120, 120, 180, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_MESSAGE_ON           = {200, 200, 255, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_OPTION_DISABLED      = {60, 60, 100, SDL_ALPHA_OPAQUE};
static const SDL_Color COLOR_OPTION_HIGHLIGHTED   = {100, 170, 255, SDL_ALPHA_OPAQUE};

#endif