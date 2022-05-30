#include "io.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* Reads processes from specified file and returns them as a list of processes.
 * Assumes no malformed input */
ListProcess *read_processes(char *filepath) {
    // Open file
    FILE *fp = fopen(filepath, "r");
    assert(fp);

    ListProcess *processes = NULL;
    int process_id, file_id1, file_id2;
    while (fscanf(fp, "%d %d %d", &process_id, &file_id1, &file_id2) == 3) {
        processes = appendListProcess(
            processes, newProcess(process_id, file_id1, file_id2));
    }

    fclose(fp);
    return processes;
}