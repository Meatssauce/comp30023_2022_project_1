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
#ifndef _LIST_H
#define _LIST_H

#include <stdbool.h>

typedef struct List {
    void *item;
    struct List *next;
} List;

List *newlist(void *item);

void freeList(List *list);

int length(List *head);

List *appendList(List *head, void *item);

List *copyList(List *head);

/* int list */

typedef struct ListInt {
    int item;
    struct ListInt *next;
} ListInt;

ListInt *newlistInt(int item);

void freeListInt(ListInt *list);

int lengthInt(ListInt *head);

ListInt *appendListInt(ListInt *head, int item);

bool isInListInt(ListInt *head, int item);

void printListInt(ListInt *head);

ListInt *copyListInt(ListInt *head);

int minListInt(ListInt *head);

int maxListInt(ListInt *head);

/* Process list */

typedef struct Process {
    int id;
    int locked;
    int requested;
    bool graphed;
} Process;

Process *newProcess(int id, int locked, int requested);

typedef struct ListProcess {
    struct Process *item;
    struct ListProcess *next;
} ListProcess;

ListProcess *newlistProcess(Process *item);

void freeListProcess(ListProcess *list);

int lengthProcess(ListProcess *head);

/* Find number of unique processes in a list of processes. */
int lengthProcessUnique(ListProcess *head);

ListProcess *appendListProcess(ListProcess *head, Process *item);

bool isInListProcess(ListProcess *head, Process *item);

ListProcess *copyListProcess(ListProcess *head);

/* Removes a process from a list of processes */
ListProcess *removeListProcess(ListProcess *head, Process *item);

#endif