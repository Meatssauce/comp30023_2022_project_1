#include "stack.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

StackInt *newStackInt(int item) {
    StackInt *my_stack = (StackInt *)malloc(sizeof(StackInt));
    my_stack->head = newlistInt(item);
    return my_stack;
}

void freeStackInt(StackInt *my_stack) {
    freeListInt(my_stack->head);
    free(my_stack);
}

/* Returns and removes first item from stack */
int popStackInt(StackInt *my_stack) {
    int head_item = my_stack->head->item;
    ListInt *old_head = my_stack->head;
    my_stack->head = my_stack->head->next;
    free(old_head);
    return head_item;
}

bool isEmptyStackInt(StackInt *my_stack) { return (my_stack->head == NULL); }

/* Append a new item to the stack */
StackInt *appendStackInt(StackInt *my_stack, int item) {
    if (my_stack == NULL) {
        return newStackInt(item);
    }

    ListInt *new_head = newlistInt(item);
    new_head->next = my_stack->head;
    my_stack->head = new_head;
    return my_stack;
}

void printStackInt(StackInt *my_stack) {
    if (my_stack == NULL) {
        return;
    }

    for (ListInt *curr = my_stack->head; curr != NULL; curr = curr->next) {
        printf("%d ", curr->item);
    }
}

/* Tests */

void testStack() {
    printf("Testing stack\n");

    StackInt *my_stack = newStackInt(0);
    my_stack = appendStackInt(my_stack, 1);
    int popped = popStackInt(my_stack);
    my_stack = appendStackInt(my_stack, 2);

    printStackInt(my_stack);
    printf("\n%d\n", popped);

    freeStackInt(my_stack);
}

// int main() {
//     testStack();
// }