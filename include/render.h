// render.h
#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "paddle.h"
#include "ball.h"

// Function declarations
static void set_sdl_color(SDL_Renderer *renderer, const SDL_Color *color);
static void set_rect(SDL_Rect *r, short x, short y, short w, short h);
SDL_Color get_flashing_color(SDL_Color color_on, SDL_Color color_off);
void draw_background(SDL_Renderer *renderer, const SDL_Color *bg_color);
void draw_start_menu(SDL_Renderer *renderer, const SDL_Color *color);
void DrawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius);
void draw_ball(SDL_Renderer *renderer, Ball *ball);
void draw_paddle(SDL_Renderer *renderer, Paddle *paddle);
SDL_Texture *get_score_texture(SDL_Renderer *renderer,
                               TTF_Font     *font, 
                               const char   *text, 
                               SDL_Color    color,
                               int          *out_w,
                               int          *out_h);
void draw_score(SDL_Renderer *renderer, Score *score, TTF_Font *font);
void draw_message(SDL_Renderer *renderer,
                  char *msg, 
                  int y,
                  TTF_Font *font, 
                  SDL_Color color); 

#endif