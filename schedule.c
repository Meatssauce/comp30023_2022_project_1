#include "schedule.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// /* Find longst path from a given node in a directed acyclic graph. Return a list
//  * of file ids in the correct order */
// ListInt *_findLongestPath(Node *graph, Node *curr) {
//     if (curr == NULL) {
//         return NULL;
//     } else if (curr->edges == NULL) {
//         return newlistInt(curr->id);
//     }

//     // find longest path out of all children
//     int max_length = 0;
//     ListInt *longest_child_path = NULL;
//     Node *edge = getNode(graph, curr->edges->item);
//     while (edge != NULL) {
//         ListInt *new_longest_child_path = _findLongestPath(graph, edge);
//         int new_length = lengthInt(new_longest_child_path);
//         if (new_length > max_length) {
//             max_length = new_length;
//             longest_child_path = new_longest_child_path;
//         }

//         if (edge->edges == NULL) {
//             break;
//         } else {
//             edge = getNode(graph, edge->edges->item);
//         }
//     }

//     ListInt *longest_path = newlistInt(curr->id);
//     for (ListInt *child = longest_child_path; child != NULL;
//          child = child->next) {
//         longest_path = appendListInt(longest_path, child->item);
//     }

//     return longest_path;
// }

// /* Find longest path in a directed acyclic graph. Return a list of file ids in
//  * the correct order */
// ListInt *findLongestPath(Node *graph) {
//     if (graph == NULL) {
//         return NULL;
//     }

//     ListInt *longest_path = NULL;
//     for (Node *curr = graph; curr != NULL; curr = curr->next) {
//         ListInt *new_longest_path = _findLongestPath(graph, curr);
//         if (lengthInt(new_longest_path) > lengthInt(longest_path)) {
//             longest_path = new_longest_path;
//         }
//     }
//     return longest_path;
// }

/* Check if a given process uses any file that is used by another process in a
 * given list */
bool _hasReusedFile(ListProcess *framed, Process *subject) {
    for (ListProcess *process = framed; process != NULL;
         process = process->next) {
        Process *process_ = process->item;
        if (process_->locked == subject->locked ||
            process_->locked == subject->requested ||
            process_->requested == subject->locked ||
            process_->requested == subject->requested) {
            return true;
        }
    }
    return false;
}

/* Schedule processes to avoid deadlock and minimise execution time, assuming no
 * file is locked yet and both files are processed and released in 1 time unit
 * once both are locked by a processes. Returns a list of process lists,
 * representing processes scheduled at each time unit, in chronological order */
List *schedule(ListProcess *unscheduled) {
    List *frames = NULL;
    ListProcess *scheduled = NULL;

    for (ListProcess *curr = unscheduled; curr != NULL; curr = curr->next) {
        Process *chosen_process = curr->item;

        if (isInListProcess(scheduled, chosen_process)) {
            continue;
        }
        scheduled = appendListProcess(scheduled, chosen_process);

        /* Add all unscheduled processes that can be done in parallel with
         * chosen_process to current frame */
        ListProcess *frame = newlistProcess(chosen_process);
        for (ListProcess *curr_j = unscheduled; curr_j != NULL; curr_j = curr_j->next) {
            Process *candidate = curr_j->item;

            if (isInListProcess(scheduled, candidate)) {
                continue;
            }

            if (!_hasReusedFile(frame, candidate)) {
                // can process candidate in parallel, move it from unscheduled
                // to frame
                scheduled = appendListProcess(scheduled, candidate);
                frame = appendListProcess(frame, candidate);
            }
        }
        frames = appendList(frames, frame);
    }

    return frames;
}

/* Tests */

// void testLongestPath() {
//     Node *graph = NULL;
//     graph = addEdgeSingle(graph, 0, 1);
//     graph = addEdgeSingle(graph, 4, 2);
//     graph = addEdgeSingle(graph, 2, 3);
//     graph = addEdgeSingle(graph, 1, 2);
//     printNodes(graph);
//     printListInt(findLongestPath(graph));
// }

// int main() {
//     testLongestPath();
// }