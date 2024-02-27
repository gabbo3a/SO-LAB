#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "utility.h"
#include "semaphore_m.h"

// Explanation: Modulo di gestione dei semafori che uìin compimazioni con le macro definite del suo .h
//              rende utilizzo degli stessi molto meno stressante e confusionario.

static const char* name = ".";
static int sems_id      = -1;

// Function to init system v semaphores array 
int init_sems() {
    // Create key + Create sems
    key_t key = ftok(name, 'S');
    if (key == GENERIC_ERROR)
        gerrorm("Error generating IPC key");

    sems_id = semget(key, NUMBER_SEMAPHORES, IPC_CREAT | 0666);
    if (sems_id == GENERIC_ERROR) 
        gerrorm("Error creating semaphores");

    return sems_id;
}

// Function to access id (Per esporre anche alla macro)
int get_sems_id() {
    return sems_id;
}

// Function to close system v semaphores array 
void close_sems() {
    int cflag = semctl(sems_id, IPC_RMID, 0);
    if (cflag == GENERIC_ERROR)
        gerrorm("Error removing semaphores");
}