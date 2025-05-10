#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "constants.h"
#include "paddle.h"
#include "ball.h"
#include "score.h"

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

SDL_Color get_flashing_color(SDL_Color color_on, SDL_Color color_off) {
    SDL_Color color;
    Uint32 time = SDL_GetTicks();
    if((time/250)%2 == 0) {
        color = color_on;
    } else {
        color = color_off;
    }
    return color;
}

// Draw background, first thing that will get drawn on every game cycle
void draw_background(SDL_Renderer *renderer, const SDL_Color *bg_color) {
    set_sdl_color(renderer, bg_color);
    SDL_RenderClear(renderer);
}

// Draw the game ball
void draw_ball(SDL_Renderer *renderer, Ball *ball) {
    if(ball && ball->visible == false) return;
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

// Creates a surface, load text to it, creates a texture out of it. out_w and out_h are
// optional and if provided, then width and height will be computed based on surface
SDL_Texture *get_texture(SDL_Renderer *renderer,
                         TTF_Font     *font,
                         const char   *text, 
                         SDL_Color    color,
                         int          *out_w,
                         int          *out_h)
{
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;

    // Create surface using the text
    surface = TTF_RenderText_Blended(font, text, 0, color);
    if (!surface) {
        SDL_Log("Unable to create surface\n");
        return NULL;
    }

    // Create texture from that surface
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(!texture) {
        SDL_Log("Unable to create texture\n");
    }

    // optionally, grab the width and height
    if (out_w) *out_w = surface->w;
    if (out_h) *out_h = surface->h;

    SDL_DestroySurface(surface);
    return texture;
}

void draw_score(SDL_Renderer *renderer, Score *score, TTF_Font *font) {
    // Prepare to render text
    char score_buffer [16];
    sprintf(score_buffer,"%d",score->score);

    // Call reusable render_text() to draw it
    int text_w = 0, text_h = 0;
    SDL_Texture *texture = get_texture(renderer,
                                        font,
                                        score_buffer, 
                                        *score->color,
                                        &text_w,
                                        &text_h);
    if (!texture) {
        SDL_Log("failed to render score text");
        return;
    }

    score->bounds.w = text_w;
    score->bounds.h = text_h;
    
    SDL_RenderTexture(renderer, texture, NULL, &score->bounds);
    SDL_DestroyTexture(texture);
}

// All messages are centered along the x, but caller can choose y axis
void draw_message(SDL_Renderer *renderer,
                  char *msg,
                  int y,
                  TTF_Font *font,
                  SDL_Color color) 
{
    // Call reusable render_text() to draw it
    int text_w = 0, text_h = 0;
    SDL_Texture *texture = get_texture(renderer,
                                       font,
                                       msg, 
                                       color,
                                       &text_w,
                                       &text_h);
    if (!texture) {
        SDL_Log("failed to render score text");
        return;
    }

    int text_x = (SDL_WINDOW_WIDTH - text_w) / 2;
    int text_y = y;
    
    SDL_FRect r = (SDL_FRect){(float) text_x, (float) text_y, (float) text_w, (float) text_h};
    SDL_RenderTexture(renderer, texture, NULL, &r);
    SDL_DestroyTexture(texture);
}
