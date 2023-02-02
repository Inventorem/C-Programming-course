//
// Created by leo on 24.12.2021.
//

#ifndef LAB4_STACK_H
#define LAB4_STACK_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 1000
#define syntax "syntax error"

typedef struct {
    int arr[MAX_STACK_SIZE];
    int top;
} stack;

int pop(stack *stack_pointer) {
    if (stack_pointer->top < 0) {
        printf(syntax);
        exit(0);
    }
    return stack_pointer->arr[stack_pointer->top--];
}

void push(stack *stack_pointer, int element) {
    stack_pointer->arr[++stack_pointer->top] = element;
}


int peek(stack *stack_pointer) {
    if (stack_pointer->top < 0) {
        printf(syntax);
        exit(0);
    }
    return stack_pointer->arr[stack_pointer->top];
}

#endif //LAB4_STACK_H
