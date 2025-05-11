// score.c
#include "score.h"
#include <stdlib.h>

Score *score_create(const SDL_Color *color) {
    Score *score = calloc(1,sizeof(Score));
    if(score == NULL)
        return NULL;
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