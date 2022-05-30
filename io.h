#ifndef _IO_H
#define _IO_H

#include "list.h"

/* Reads processes from specified file and returns them as a list of processes.
 * Assumes no malformed input */
ListProcess *read_processes(char *filepath);

#endif