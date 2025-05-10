// score.c
#include "score.h"
#include <stdlib.h>

Score *create_score(int x, int y, int w, int h, const SDL_Color *color) {
    Score *score = malloc(sizeof(Score));
    if(score == NULL)
        return NULL;
    score->bounds = (SDL_FRect){ (float) x, (float) y, (float) w, (float) h }; 
    score->score  = 0;
    score->color  = color;
    return score;
}

void increase_score(Score *score) {
    score->score++;
}

void reset_score(Score *score) {
    score->score = 0;
}