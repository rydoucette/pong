// gamestate_stack.h
#ifndef GAMESTATE_STACK_H
#define GAMESTATE_STACK_H

#include <stdbool.h>
#include "gamestate.h"

typedef struct {
    GameState **states;
    int num_items;
    int size;
} GameStateStack;

bool gamestate_stack_create(GameStateStack *stack, int size);
void gamestate_stack_destroy(GameStateStack *stack);
bool gamestate_stack_push(GameStateStack *stack, GameState *state);
bool gamestate_stack_pop(GameStateStack *stack, GameState **state);
bool gamestate_stack_peek(GameStateStack *stack, GameState **state);
bool gamestate_stack_is_empty(GameStateStack *stack);

#endif