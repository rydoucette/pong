// score.h
#ifndef SCORE_H
#define SCORE_H

#include <SDL3/SDL.h>

typedef struct {
    SDL_FRect bounds;
    int       score;
    const     SDL_Color *color;
} Score;

Score *create_score(float x, float y, const SDL_Color *color);
void   increase_score(Score *score);
void   reset_score(Score *score);

#endif