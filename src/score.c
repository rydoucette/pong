// score.c
#include "score.h"
#include <stdlib.h>

Score *create_score(float x, float y, const SDL_Color *color) {
    Score *score = malloc(sizeof(Score));
    if(score == NULL)
        return NULL;
    score->bounds = (SDL_FRect){ x, y, (float) 0, (float) 0 }; 
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