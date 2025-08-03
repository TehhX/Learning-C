#ifndef STACK_H
#define STACK_H

#include "node.h"

// TODO

/*
General declarations:
---------------------

struct stack_t {
    node *head = NULL;
};

void stack_push(stack_t *stack, T val) {
    node_t *newHead = malloc(sizeof(node_t));
    newHead->val = val;
    newHead->next = stack->head;
    stack->head = newHead;
}

T stack_pop(stack_t *stack) {
    T val = stack->head->val;
    node_t *newHead = stack->head->next;
    free(stack->head);
    stack->head = newHead;
    return val;
}

T stack_peek(stack_t *stack) {
    return stack->head->val;
}

void stack_free(stack_t *stack) {
    node_t *traveller = stack->head;
    
}
*/

#define __DEFINE_STACK_STRUCT(T) typedef struct stack_##T##_t { node_##T##_t *head; } stack_##T##_t;
#define   __DEFINE_STACK_PUSH(T) void stack_##T##_push(stack_##T##_t *stack, T val) {  }
#define    __DEFINE_STACK_POP(T) T stack_##T##_pop(stack_##T##_t *stack) {  }
#define   __DEFINE_STACK_PEEK(T) T stack_##T##_peek(stack_##T##_t *stack) {  }
#define   __DEFINE_STACK_FREE(T) void stack_##T##_free(stack_##T##_t *stack) {  }

// Use this to declare a stack containing data of type T. Make sure T contains no whitespace. If it does, typedef it to remove whitespace e.g. "long long" -> "ll_t".
#define DEFINE_STACK(T) __DEFINE_STACK_STRUCT(T) __DEFINE_STACK_PUSH(T) __DEFINE_STACK_POP(T) __DEFINE_STACK_PEEK(T) __DEFINE_STACK_FREE(T)

#endif // STACK_H
