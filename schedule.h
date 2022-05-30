#ifndef _SCHEDULE_H
#define _SCHEDULE_H

#include "graph.h"
#include "list.h"

// /* Find longest path in a directed acyclic graph. Return a list of file ids in
//  * the correct order */
// ListInt *findLongestPath(Node *graph);

/* Schedule processes to avoid deadlock and minimise execution time, assuming no
 * file is locked yet and both files are processed and released in 1 time unit
 * once both are locked by a processes. Returns a list of process lists,
 * representing processes scheduled at each time unit, in chronological order */
List *schedule(ListProcess *unscheduled);

#endif