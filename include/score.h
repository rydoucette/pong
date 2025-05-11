// score.h
#ifndef SCORE_H
#define SCORE_H

#include <SDL3/SDL.h>

/*
 * For future ryan - since struct is declared here, the struct properties are available
 * to anything that includes this header file (public). Consider moving the struct 
 * declaration to the .c file (private) and have getters and setters as needed
 */

typedef struct {
    SDL_FRect bounds;
    int       score;
    const     SDL_Color *color;
} Score;

Score *score_create(float x, float y, const SDL_Color *color);
void   increase_score(Score *score);
void   reset_score(Score *score);

#endif