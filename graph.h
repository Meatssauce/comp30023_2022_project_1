#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdbool.h>

#include "list.h"
#include "stack.h"
// incluce listnstack.h doesn't work

typedef struct Request {
    int process_id, file_id;
} Request;

typedef struct Node {
    int id;
    ListInt *edges;
    struct Node *next;
} Node;

void freeNodes(Node *graph);

/* Create a new graph */
Node *newNode(int id);

/* Add a new node to a graph */
Node *addNode(Node *head, int id);

/* Get node by id, returns NULL if not found */
Node *getNode(Node *head, int id);

/* Check if two nodes are connected via BFS */
bool isConnected(Node *src, Node *dst);

/* Check if a node is in a graph */
bool isInGraph(Node *head, int subject_id);

/* Add a single directional edge from a node to a node. Both nodes muct be in
 * the graph.*/
Node *addEdgeSingle(Node *graph, int src_id, int dst_id);

/* Keep only the cycling part of an int list that contains a cycle. e.g. 1,2,3,2
 * => 2,3,2" */
ListInt *prune_cycle(ListInt *head, int cutoff);

/* Find all cycles in a graph. Accepts disjoint graph. Returns list of int
 * lists*/
List *findAllCycles(Node *head);

/* Print all nodes in a graph and their edges */
void printNodes(Node *head);

/* Returns a reversed directed graph */
Node *reverseGraph(Node *graph);

/* Build a disjoint wait-for graph from a list or processes */
Node *buildGraphFromProcesses(ListProcess *processes);

#endif