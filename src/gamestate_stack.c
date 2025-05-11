#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "gamestate_stack.h"

// Create a stack of gamestates
bool gamestate_stack_create(GameStateStack *stack, int size) {
    stack->num_items = 0;
    stack->size = size;
    stack->states = (GameState **) malloc(size * sizeof(GameState *));
    if(stack->states == NULL) {
        SDL_Log("Could not allocate memory.\n");
        return false;
    }
    return true;
}

// Free the stack from memory
void gamestate_stack_destroy(GameStateStack *stack) {
    free(stack->states);
    stack->states = NULL;
    stack->num_items = 0;
    stack->size = 0;
}

// Push a game state on to the stack
bool gamestate_stack_push(GameStateStack *stack, GameState *state) {
    if(stack->num_items >= stack->size) {
        stack->size *= 2;
        GameState **temp = (GameState **) realloc(stack->states, 
                                                  stack->size * sizeof(GameState *));
        if(temp == NULL) {
            SDL_Log("Could not reallocate memory\n");
            return false;
        }
        stack->states = temp;
    }
    stack->states[stack->num_items++] = state;
    return true;
}

// Take a gamestate from the top of the stack
bool gamestate_stack_pop(GameStateStack *stack, GameState **state) {
    if(gamestate_stack_is_empty(stack)) {
        SDL_Log("CANNOT POP OFF AN EMPTY STACK\n");
        return false;
    }
    *state = stack->states[--stack->num_items];
    return true;
}

// Get but do not remove a state at the top of the stack
bool gamestate_stack_peek(GameStateStack *stack, GameState **state) {
    if(gamestate_stack_is_empty(stack)) {
        SDL_Log("CANNOT READ OFF AN EMPTY STACK\n");
        return false;
    }
    *state = stack->states[stack->num_items-1];
    return true;
}

// Check if there are any states in the stack
bool gamestate_stack_is_empty(GameStateStack *stack) {
    return stack->num_items == 0;
}