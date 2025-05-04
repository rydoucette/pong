#include <stdio.h>
#include <SDL3/SDL.h>
#include "constants.h"
#include "paddle.h"
#include "ball.h"

// Helper function to set the rendering color
static void set_sdl_color(SDL_Renderer *renderer, const SDL_Color *color) {
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
}

// Internal representation of game is done with Game Height/Game Width, this function 
// scales things up for a better fit to the window
static void set_rect(SDL_FRect *r) {
    r->x *= (float) BLOCK_SIZE_IN_PIXELS;
    r->y *= (float) BLOCK_SIZE_IN_PIXELS;
    r->w *= (float) BLOCK_SIZE_IN_PIXELS;
    r->h *= (float) BLOCK_SIZE_IN_PIXELS;
}

// Draw ba`kground, first thing that will get drawn on every game cycle
void draw_background(SDL_Renderer *renderer, const SDL_Color *bg_color) {
    set_sdl_color(renderer, bg_color);
    SDL_RenderClear(renderer);
}

// Draw the game ball
void draw_ball(SDL_Renderer *renderer, Ball *ball) {
    SDL_FRect r = ball->bounds;
    set_sdl_color(renderer, ball->color);
    set_rect(&r);
    SDL_RenderFillRect(renderer, &r);
}

// Draw a paddle object
void draw_paddle(SDL_Renderer *renderer, Paddle *paddle) {
    SDL_FRect r = paddle->bounds;
    set_sdl_color(renderer, paddle->color);
    set_rect(&r);
    SDL_RenderFillRect(renderer, &r);
}