/*
list.c
data struture and functions
    List
    newlist()
    freeList()
are inspired by

"Implementations for helper functions for linked list construction and
manipulation.

by Grady Fitzpatrick for COMP20007 Assignment 1 2021"
*/
#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

List *newlist(void *item) {
    List *head = (List *)malloc(sizeof(List));
    assert(head);
    head->item = item;
    head->next = NULL;
    return head;
}

void freeList(List *list) {
    List *next;
    /* Iterate through list until the end of the list (NULL) is reached. */
    for (next = list; list != NULL; list = next) {
        /* Store next pointer before we free list's space. */
        next = list->next;
        free(list);
    }
}

int length(List *head) {
    int i = 0;
    for (List *curr = head; curr != NULL; curr = curr->next) {
        i++;
    }
    return i;
}

List *appendList(List *head, void *item) {
    if (head == NULL) {
        return newlist(item);
    }

    List *prev, *curr;
    for (curr = head; curr != NULL; curr = curr->next) {
        prev = curr;
    }
    prev->next = newlist(item);
    return head;
}

List *copyList(List *head) {
    List *new_head = NULL;
    for (List *curr = head; curr != NULL; curr = curr->next) {
        new_head = appendList(new_head, curr->item);
    }
    return new_head;
}

/* int list */

ListInt *newlistInt(int item) {
    ListInt *head = (ListInt *)malloc(sizeof(ListInt));
    assert(head);
    head->item = item;
    head->next = NULL;
    return head;
}

void freeListInt(ListInt *list) {
    ListInt *next;
    /* Iterate through list until the end of the list (NULL) is reached. */
    for (next = list; list != NULL; list = next) {
        /* Store next pointer before we free list's space. */
        next = list->next;
        free(list);
    }
}

int lengthInt(ListInt *head) {
    int i = 0;
    for (ListInt *curr = head; curr != NULL; curr = curr->next) {
        i++;
    }
    return i;
}

ListInt *appendListInt(ListInt *head, int item) {
    if (head == NULL) {
        return newlistInt(item);
    }

    ListInt *prev, *curr;
    for (curr = head; curr != NULL; curr = curr->next) {
        prev = curr;
    }
    prev->next = newlistInt(item);
    return head;
}

bool isInListInt(ListInt *head, int item) {
    ListInt *curr;
    for (curr = head; curr != NULL; curr = curr->next) {
        if (curr->item == item) {
            return true;
        }
    }
    return false;
}

void printListInt(ListInt *head) {
    for (ListInt *curr = head; curr != NULL; curr = curr->next) {
        printf("%d ", curr->item);
    }
    printf("\n");
}

ListInt *copyListInt(ListInt *head) {
    ListInt *new_head = NULL;
    for (ListInt *curr = head; curr != NULL; curr = curr->next) {
        new_head = appendListInt(new_head, curr->item);
    }
    return new_head;
}

int minListInt(ListInt *head) {
    assert(head);
    int min_int = head->item;
    for (ListInt *curr = head->next; curr != NULL; curr = curr->next) {
        if (curr->item < min_int) {
            min_int = curr->item;
        }
    }
    return min_int;
}

int maxListInt(ListInt *head) {
    assert(head);
    int max_int = head->item;
    for (ListInt *curr = head->next; curr != NULL; curr = curr->next) {
        if (curr->item > max_int) {
            max_int = curr->item;
        }
    }
    return max_int;
}

/* process list */

Process *newProcess(int id, int locked, int requested) {
    Process *new_process = malloc(sizeof(Process));
    new_process->id = id;
    new_process->locked = locked;
    new_process->requested = requested;
    new_process->graphed = false;
    return new_process;
}

ListProcess *newlistProcess(Process *item) {
    ListProcess *head = (ListProcess *)malloc(sizeof(ListProcess));
    assert(head);
    head->item = item;
    head->next = NULL;
    return head;
}

void freeListProcess(ListProcess *list) {
    ListProcess *next;
    /* Iterate through list until the end of the list (NULL) is reached. */
    for (next = list; list != NULL; list = next) {
        /* Store next pointer before we free list's space. */
        next = list->next;
        free(list);
    }
}

int lengthProcess(ListProcess *head) {
    int i = 0;
    for (ListProcess *curr = head; curr != NULL; curr = curr->next) {
        i++;
    }
    return i;
}

/* Find number of unique processes in a list of processes. */
int lengthProcessUnique(ListProcess *head) {
    int i = 0;
    ListInt *seen = NULL;
    for (ListProcess *curr = head; curr != NULL; curr = curr->next) {
        if (isInListInt(seen, curr->item->id)) {
            continue;
        }
        seen = appendListInt(seen, curr->item->id);
        i++;
    }
    return i;
}

ListProcess *appendListProcess(ListProcess *head, Process *item) {
    if (head == NULL) {
        return newlistProcess(item);
    }

    ListProcess *prev, *curr;
    for (curr = head; curr != NULL; curr = curr->next) {
        prev = curr;
    }
    prev->next = newlistProcess(item);
    return head;
}

bool isInListProcess(ListProcess *head, Process *item) {
    ListProcess *curr;
    for (curr = head; curr != NULL; curr = curr->next) {
        if (curr->item->id == item->id) {
            return true;
        }
    }
    return false;
}

ListProcess *copyListProcess(ListProcess *head) {
    ListProcess *new_head = NULL;
    for (ListProcess *curr = head; curr != NULL; curr = curr->next) {
        new_head = appendListProcess(new_head, curr->item);
    }
    return new_head;
}

/* Removes a process from a list of processes */
ListProcess *removeListProcess(ListProcess *head, Process *item) {
    ListProcess *prev = NULL, *curr;
    for (curr = head; curr != NULL; curr = curr->next) {
        if (curr->item == item) {
            if (prev == NULL) {
                head = curr->next;
            } else {
                prev->next = curr->next;
            }
            return head;
        }
        prev = curr;
    }
    return head;
}

/* tests */

// void testListInt() {
//     printf("Testing list_int\n");

//     int val = 1;
//     ListInt *mylist = newlistInt(val);
//     int val2 = 2;
//     mylist = appendListInt(mylist, val2);
//     // mylist = prependListInt(mylist, val2);
//     // int val3 = 3;

//     ListInt *mylist2 = newlistInt(val);
//     for (int i = 0; i < 5; i++) {
//         mylist2 = appendListInt(mylist2, i);
//     }

//     printListInt(mylist);

//     freeListInt((mylist));
// }

// int main() {
//     testListInt();
// }
