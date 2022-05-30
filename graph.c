#include "graph.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// implement graph as a linked list of linked lists
// add connected node by finding the right node and append to the list at that
// node add new node by appending new node to graph

void freeNodes(Node *graph) {
    Node *curr;
    for (curr = graph; graph != NULL; graph = curr) {
        curr = graph->next;
        free(graph);
    }
}

/* Create a new graph */
Node *newNode(int id) {
    Node *head = (Node *)malloc(sizeof(Node));
    assert(head);
    head->id = id;
    head->edges = NULL;
    head->next = NULL;
    return head;
}

/* Add a new node to a graph */
Node *addNode(Node *head, int id) {
    if (head == NULL) {
        return newNode(id);
    } else if (isInGraph(head, id)) {
        return head;
    }

    Node *prev, *curr;
    for (curr = head; curr != NULL; curr = curr->next) {
        prev = curr;
    }
    prev->next = newNode(id);
    return head;
}

/* Get node by id, returns NULL if not found */
Node *getNode(Node *head, int id) {
    for (Node *curr = head; curr != NULL; curr = curr->next) {
        if (curr->id == id) {
            return curr;
        }
    }
    return NULL;
}

/* Check if two nodes are connected via BFS */
bool isConnected(Node *src, Node *dst) {
    ListInt *visited = NULL;
    ListInt *queue = newlistInt(src->id);

    for (ListInt *curr = queue; curr != NULL; curr = curr->next) {
        if (isInListInt(visited, curr->item)) {
            continue;
        }

        // add node's edges to end of queue
        ListInt *edges = getNode(src, curr->item)->edges;
        for (ListInt *e = edges; e != NULL; e = e->next) {
            queue = appendListInt(queue, e->item);
        }
        // mark node as visited
        visited = appendListInt(visited, curr->item);

        if (curr->item == dst->id) {
            // reached dst node, return true
            return true;
        }
    }
    // did not reach dst node, return false
    return false;
}

/* Check if a node is in a graph */
bool isInGraph(Node *head, int subject_id) {
    for (Node *curr = head; curr != NULL; curr = curr->next) {
        if (curr->id == subject_id) {
            return true;
        }
    }
    return false;
}

/* Add a single directional edge from a node to a node. Add new nodes as needed
 */
Node *addEdgeSingle(Node *graph, int src_id, int dst_id) {
    if (graph == NULL) {
        graph = newNode(src_id);
    }
    // add new nodes as needed
    else if (!isInGraph(graph, src_id)) {
        graph = addNode(graph, src_id);
    }
    if (!isInGraph(graph, dst_id)) {
        graph = addNode(graph, dst_id);
    }

    Node *src = getNode(graph, src_id);
    assert(src);
    if (src->edges == NULL) {
        src->edges = newlistInt(dst_id);
    } else if (!isInListInt(src->edges, dst_id)) {
        src->edges = appendListInt(src->edges, dst_id);
    }

    return graph;
}

/* Keep only the cycling part of an int list that contains a cycle. e.g. 1,2,3,2
 * => 2,3,2" */
ListInt *prune_cycle(ListInt *head, int cutoff) {
    for (ListInt *curr = head; curr != NULL; curr = curr->next) {
        if (curr->item == cutoff) {
            return curr;
        }
    }
    return NULL;
}

/* Find all cycles in a graph. Accepts disjoint graph. Returns list of int
 * lists (need to cast list item from void *) or NULL if none found */
List *findAllCycles(Node *head) {
    ListInt *visited = NULL;
    List *cycles = NULL;

    for (Node *src = head; src != NULL; src = src->next) {
        if (isInListInt(visited, src->id)) {
            // already traversed pass scr, don't traverse again
            continue;
        }

        // traverse from src
        StackInt *stack = newStackInt(src->id);
        ListInt *path = NULL;

        for (int curr = popStackInt(stack);; curr = popStackInt(stack)) {
            bool has_visited = isInListInt(visited, curr);
            bool is_in_path = isInListInt(path, curr);

            if (!has_visited && !is_in_path) {
                // reach brand new node

                // add node's edges to stack
                ListInt *edges = getNode(head, curr)->edges;
                for (ListInt *e = edges; e != NULL; e = e->next) {
                    stack = appendStackInt(stack, e->item);
                }
                // mark node as visited
                visited = appendListInt(visited, curr);
                // add node to path
                path = appendListInt(path, curr);
            } else if (has_visited && !is_in_path) {
                // path from current src reached a previously visited path, stop
                // traversing from src
                break;
            } else if (has_visited && is_in_path) {
                // cycle detected, append cyclic part in path to cycles list and
                // move on
                ListInt *cycle = copyListInt(prune_cycle(path, curr));
                cycles = appendList(cycles, cycle);
                break;
            } else {
                // impossible state (!has_visited && is_in_path), crash program
                printf("Impossible DFS state");
                return cycles;
            }

            if (isEmptyStackInt(stack)) {
                break;
            }
        }
    }

    return cycles;
}

/* Print all nodes in a graph and their edges */
void printNodes(Node *head) {
    for (Node *curr = head; curr != NULL; curr = curr->next) {
        printf("%d | ", curr->id);
        printListInt(curr->edges);
    }
}

/* Returns a reversed directed graph */
Node *reverseGraph(Node *graph) {
    Node *reverse_graph = NULL;
    for (Node *curr = graph; curr != NULL; curr = curr->next) {
        if (curr->edges == NULL) {
            reverse_graph = addNode(reverse_graph, curr->id);
        }
        for (ListInt *edge = curr->edges; edge != NULL; edge = edge->next) {
            reverse_graph = addEdgeSingle(reverse_graph, edge->item, curr->id);
        }
    }
    return reverse_graph;
}

// /* Returns the Process from a list of processes that's blocking a given
// Process */ Process *findBlocker(ListProcess *processes, Process *blocked) {
//     for (ListProcess *curr = processes; curr != NULL; curr = curr->next) {
//         if (curr->item->locked == blocked->requested) {
//             return curr->item;
//         }
//     }
//     return NULL;
// }

// /* Build a disjoint wait-for graph from a list of processes */
// Node *buildGraphFromProcesses(ListProcess *processes) {
//     Node *graph = NULL;
//     ListProcess *graphed_processes = NULL;
//     for (ListProcess *curr = processes; curr != NULL; curr = curr->next) {
//         Process *process = curr->item;

//         if (isInListProcess(graphed_processes, process)) {
//             // already graphed this Process and its edges
//             continue;
//         }

//         Process *blocking_process;
//         if ((blocking_process = findBlocker(processes, process)) != NULL) {
//             // Process waiting for another, add both to graph
//             graph = addEdgeSingle(graph, process->id, blocking_process->id);
//         } else if (!isInGraph(graph, process->id)) {
//             // Process not waiting for another, add it to graph
//             graph = addNode(graph, process->id);
//         }

//         graphed_processes = appendListProcess(graphed_processes, process);
//     }
//     return graph;
// }

/* Build a disjoint wait-for graph of files from a list of processes */
Node *buildGraphFromProcesses(ListProcess *processes) {
    Node *graph = NULL;
    ListProcess *graphed_processes = NULL;
    for (ListProcess *curr = processes; curr != NULL; curr = curr->next) {
        Process *process = curr->item;
        if (isInListProcess(graphed_processes, process)) {
            // already graphed this process and its edges
            continue;
        }
        graph = addEdgeSingle(graph, process->locked, process->requested);
        graphed_processes = appendListProcess(graphed_processes, process);
    }
    return graph;
}

/* Tests */

// void testGraphUtils() {
//     printf("Testing graph utils\n");

//     Node *graph = NULL;
//     graph = addEdgeSingle(graph, 1, 2);
//     graph = addEdgeSingle(graph, 2, 3);
//     graph = addEdgeSingle(graph, 3, 2);
//     graph = addEdgeSingle(graph, 0, 2);

//     // printNodes(graph);

//     // ListInt *cycle;
//     // if ((cycle = findCycle(graph)) != NULL) {
//     //     printf("Cycle detected\n");
//     //     printListInt(cycle);
//     //     printf("\n");
//     // } else {
//     //     printf("No cycle detected\n");
//     // }

//     graph = addEdgeSingle(graph, 4, 5);
//     graph = addEdgeSingle(graph, 5, 6);
//     graph = addEdgeSingle(graph, 6, 4);

//     printNodes(graph);

//     list *cycles;
//     if ((cycles = findAllCycles(graph)) != NULL) {
//         printf("Cycle detected\n");
//         for (list *curr = cycles; curr != NULL; curr = curr->next) {
//             ListInt *cycle = (ListInt *)curr->item;
//             printListInt(cycle);
//         }
//     } else {
//         printf("No cycle detected\n");
//     }

//     /* Haven't tested against multiple edges per node yet */
// }

// void testBuildGraph() {
//     printf("Testing build graph\n");

//     ListProcess *processes = NULL;
//     processes = appendListProcess(processes, newProcess(101, 1, 2));
//     processes = appendListProcess(processes, newProcess(102, 2, 3));
//     processes = appendListProcess(processes, newProcess(103, 3, 2));
//     processes = appendListProcess(processes, newProcess(104, 0, 2));
//     processes = appendListProcess(processes, newProcess(105, 4, 5));
//     processes = appendListProcess(processes, newProcess(106, 5, 6));
//     processes = appendListProcess(processes, newProcess(107, 6, 4));

//     Node *graph = buildGraphFromProcesses(processes);
//     printNodes(graph);
// }

// int main() {
//     testGraphUtils();
//     testBuildGraph();
// }