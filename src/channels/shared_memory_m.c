#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#include "utility.h"
#include "shared_memory_m.h"

// Explanation: Modulo di gestione della memoria condivisa come per gli altri moduli il suo utilizzo è semplice,
//              incaspula, pero espone anche il puntatore alla memoria condivisa fondamentale per poter utilizzare.
// Nota:        Si sarebbe potuto esporre anche le funozioni tipo getter/setter ma il lavoro sarebbere stato lungo,
//              confusionario e pressoche inutile.

static const char* name      = ".";
static int shm_id            = -1;
static SHMGlobalData* gdatap = NULL;

// Function to init shared memory
void init_shm() {
    // Create key + Create shared memory
    key_t key = ftok(name, 'M');
    if (key == GENERIC_ERROR)
        gerrorm("Error generating IPC key");

    shm_id = shmget(key, sizeof(SHMGlobalData), IPC_CREAT | 0666);
    if (shm_id == GENERIC_ERROR) 
        gerrorm("Error creating shared memory");

    // Link shared memory with pointer
    gdatap = (SHMGlobalData*) shmat(shm_id, NULL, 0);
    if ((void *) gdatap == (void *) GENERIC_ERROR)
        gerrorm("Connection error shared memory");
}

// Function to shared memory
void clear_shm() {
    if (gdatap == NULL) 
        gerrorm("Error while cleaning memory");
    memset(gdatap, 0, sizeof(SHMGlobalData));
}

// Function to get gdatap
SHMGlobalData* get_shm_pointer() {
    return gdatap;
}

// Function to close + remove shared memory
void close_shm() {
    int cflag = shmctl(shm_id, IPC_RMID, NULL);
    if (cflag == GENERIC_ERROR)
        gerrorm("Error removing shared memory");
}