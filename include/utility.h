#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

#include "shared_memory_m.h"
#include "semaphore_m.h"

// Macros to error management
#define gerrorm(msg) {              \
    perror("["__FILE__ "] " msg);   \
    exit(EXIT_FAILURE);             \
}        

#define send_psignal(target_pid, signo, payload) {                        \
    sigqueue(target_pid, signo, (union sigval) {.sival_int = payload});   \
}  

#define sgerrorm(target_pid, signo, payload) { \
    send_psignal(target_pid, signo, payload)   \
    exit(EXIT_FAILURE);                        \
}                                              \

#define init {      \
    init_sems();    \
    init_db();      \
    init_shm();     \
    init_msgq();    \
}                   \

// Macros to iterare struct
#define isprint(memory, key, dtype, i)  \
    dprintf(                            \
        STDOUT_FILENO,                  \
        "%-30s: %d\n",                  \
        key,                            \
        *((dtype*) memory + i)          \
    );                                 

#define sprint(memory, keys, ktype) {                \
    size_t length = sizeof(*memory) / sizeof(ktype); \
    for (size_t i = 0; i < length; i++)              \
        isprint(memory, keys[i], ktype, i);          \
}                                                    \

#define max(a, b) ((a > b) ? a : b);

enum { GENERIC_ERROR = -1 };
enum { FORK_ERROR = -1, CHILD_PROCESS = 0 };
enum {
    TIMEOUT_TERMINATION, 
    EXPLODE_TERMINATION,
    BLACKOUT_TERMINATION,
    MELTDOWN_TERMINATION,
    GENERIC_SIGNAL_PAYLOAD
};

const char* ntos(const int n);
int get_lrand(int min, int max);
bool contain(int* array, size_t size, int value);
sigset_t* get_struct_block_all_signal(); 

#endif