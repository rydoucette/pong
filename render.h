// render.h
#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>
#include "paddle.h"
#include "ball.h"

// Function declarations
static void set_sdl_color(SDL_Renderer *renderer, const SDL_Color *color);
static void set_rect(SDL_Rect *r, short x, short y, short w, short h);
void draw_background(SDL_Renderer *renderer, const SDL_Color *bg_color);
void draw_ball(SDL_Renderer *renderer, Ball *ball);
void draw_paddle(SDL_Renderer *renderer, Paddle *paddle);
//void draw_ball(SDL_Renderer *renderer, Paddle *ball)
//void draw_score(SDL_Renderer *renderer, Paddle *score)

#endif