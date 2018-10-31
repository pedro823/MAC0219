#ifndef GLOBALS_H
#define GLOBALS_H

#include <sched.h>

#define MAX_PROCESSES 32768
#define GLOBALS_AWAIT(x) while (!(x)) sched_yield()

#endif /*GLOBALS_H*/
