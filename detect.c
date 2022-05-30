#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "io.h"
#include "list.h"
#include "stack.h"
#include "schedule.h"

/* Find id of process that is locking file for each file id in a list. Return a
 * list of process ids in the same order as the file id */
ListInt *findLockerProcessIds(ListInt *file_id_cycle, ListProcess *processes) {
    ListInt *locker_ids = NULL;
    for (ListInt *curr = file_id_cycle; curr != NULL; curr = curr->next) {
        for (ListProcess *process = processes; process != NULL; process = process->next) {
            if (process->item->locked == curr->item) {
                // found process that locks current file
                locker_ids = appendListInt(locker_ids, process->item->id);
            }
        }
    }
    return locker_ids;
}

/* Return process ids to terminate for given a deadlock */
int findTerminationId(ListInt *file_id_cycle, ListProcess *processes) {
    return minListInt(findLockerProcessIds(file_id_cycle, processes));
}

/* Return number of unique files given a list of processes */
int countFiles(ListProcess *processes) {
    int n_files = 0;
    ListInt *counted = NULL;
    for (ListProcess *curr = processes; curr != NULL; curr = curr->next) {
        int locked = curr->item->locked;
        int requested = curr->item->requested;

        if (!isInListInt(counted, locked)) {
            counted = appendListInt(counted, locked);
            n_files++;
        }
        if (!isInListInt(counted, requested)) {
            counted = appendListInt(counted, requested);
            n_files++;
        }
    }
    return n_files;
}

/* Compute time it takes until all nodes pointing to this node are unblocked,
 * assuming no deadlocks */
int computeUnblockTime(Node *graph, Node *curr) {
    if (curr == NULL) {
        return 0;
    } else if (lengthInt(curr->edges) == 0) {
        return 1;
    } else if (lengthInt(curr->edges) == 1) {
        return 1 + computeUnblockTime(graph, getNode(graph, curr->edges->item));
    }

    // Get unblock time down each branch of the multibranch node
    ListInt *unblock_times = NULL;
    for (ListInt *edge = curr->edges; edge != NULL; edge = edge->next) {
        unblock_times = appendListInt(
            unblock_times,
            computeUnblockTime(graph, getNode(graph, edge->item)));
    }

    int max_branch_time = maxListInt(unblock_times);
    int min_branch_time = minListInt(unblock_times);
    int rest = lengthInt(curr->edges) - 1 + min_branch_time;
    if (rest > max_branch_time) {
        // printf("rest %d\n", rest);
        return 1 + rest;
    } else {
        // printf("max %d\n", max_branch_time);
        return 1 + max_branch_time;
    }
}

/* Find execution time as longest path in a given acyclic wait-for graph */
int computeExecutionTime(Node *wait_for_graph, ListProcess *processes) {
    int max_time = 0;
    Node *reversed_graph = reverseGraph(wait_for_graph);
    // printf("============start reversed wait for graph============\n");
    // printNodes(reversed_graph);
    // printf("============end reversed wait for graph============\n");

    // add execution time of longest path
    int time;
    for (Node *curr = reversed_graph; curr != NULL; curr = curr->next) {
        time = computeUnblockTime(reversed_graph, curr);

        // printf("%d %d %d\n", curr->id, time, lengthInt(curr->edges));
        // printListInt(curr->edges);
        // printf("time %d", time);

        if (time > max_time) {
            max_time = time;
        }
    }

    return max_time;
}

/* main */

int main(int argc, char *argv[]) {
    bool should_find_execution_time = false;
    bool should_allocate_processes = false;
    char *filepath = NULL;

    // Process io
    if (argc < 3 || argc > 5) {
        printf("malformed script input");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            filepath = argv[++i];
        }
        if (strcmp(argv[i], "-e") == 0) {
            should_find_execution_time = true;
        }
        if (strcmp(argv[i], "-c") == 0) {
            should_allocate_processes = true;
        }
    }
    if (filepath == NULL) {
        printf("NULL file path");
        exit(EXIT_FAILURE);
    }

    ListProcess *processes = read_processes(filepath);

    printf("Processes %d\n", lengthProcessUnique(processes));
    printf("Files %d\n", countFiles(processes));

    Node *wait_for_graph = buildGraphFromProcesses(processes);
    List *cycles = NULL;

    // printf("============start wait for graph============\n");
    // printNodes(wait_for_graph);
    // printf("============end wait for graph============\n");

    if (should_find_execution_time) {
        printf("Execution time %d\n",
               computeExecutionTime(wait_for_graph, processes));
    } else {
        // Detect deadlocks
        if ((cycles = findAllCycles(wait_for_graph)) != NULL) {
            // detected deadlock
            printf("Deadlock detected\nTerminate");
            // print ids to terminate
            for (List *cycle = cycles; cycle != NULL; cycle = cycle->next) {
                printf(" %d", findTerminationId((ListInt *)cycle->item, processes));
            }
            printf("\n");
        } else {
            // no deadlock detected
            printf("No deadlocks\n");
        }
    }
    if (cycles != NULL && should_allocate_processes) {
        List *frames = schedule(processes);
        int t = 0;
        for (List *frame = frames; frame != NULL; frame = frame->next) {
            for (ListProcess *process = frame->item; process != NULL; process = process->next) {
                Process *process_ = process->item;
                printf("%d %d %d,%d\n", t, process_->id, process_->locked, process_->requested);
            }
            t++;
        }
        printf("Simulation time %d\n", t);

        freeList(frames);
    }

    freeListProcess(processes);
    freeNodes(wait_for_graph);
    freeList(cycles);

    return 0;
}