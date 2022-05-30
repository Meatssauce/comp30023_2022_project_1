#ifndef _STACK_H
#define _STACK_H

#include <stdbool.h>

#include "list.h"

typedef struct StackInt {
    ListInt *head;
} StackInt;

StackInt *newStackInt(int item);

void freeStackInt(StackInt *my_stack);

/* Returns and removes first item from stack */
int popStackInt(StackInt *my_stack);

bool isEmptyStackInt(StackInt *my_stack);

/* Append a new item to the stack */
StackInt *appendStackInt(StackInt *my_stack, int item);

void printStackInt(StackInt *my_stack);

#endif