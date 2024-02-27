#ifndef SEMM_H
#define SEMM_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "utility.h"

enum {
    FILE_DB_MUTEX,              // Index code for file db mutex  
    SHARED_MEMORY_MUTEX,        // Index code for shared memory mutex  
    START_SIM_COUNT             // Index code for initial synchronization sem
};

#define NUMBER_SEMAPHORES 3
#define NUMBER_SIM_PROCESS 3

// Macro to init all sim semaphores
#define init_sim_sems() {                                                    \
    semctl(get_sems_id(), FILE_DB_MUTEX,       SETVAL, 1);                   \
    semctl(get_sems_id(), SHARED_MEMORY_MUTEX, SETVAL, 1);                   \
    semctl(get_sems_id(), START_SIM_COUNT,     SETVAL, NUMBER_SIM_PROCESS);  \
}                                                                            \

// Macro to use P or V with semop
#define PorV(sid, op, flag) {                                                        \
    if (semop(get_sems_id(), &(struct sembuf) {sid, op, flag}, 1) == GENERIC_ERROR)  \
        gerrorm("Error in the semop operation on semaphores")                        \
}                                                                                    \

// Macro to use a simple mutex critical section with a "similar callback"
#define mutex_critical_section(MUTEX_INDEX, code) {         \
    PorV(MUTEX_INDEX, -1, 0);                               \
    code                                                    \
    PorV(MUTEX_INDEX, 1, 0);                                \
}                                                                                      

int  init_sems();
int  get_sems_id();
void close_sems();

#endif