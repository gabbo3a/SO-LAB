#ifndef SMM_H
#define SMM_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int n_activations;
    int n_splits;
    int n_energy_produced;
    int n_energy_consumed;
    int n_slag_produced;
    int n_inhibitor_energy_absorbed;
    int n_inhibitor_permits_granted;
} SHMGlobalARData;

typedef struct {
    pid_t pid_inhibitor;
    pid_t pid_activator;
    pid_t pid_master;
    pid_t pid_diet;
} SHMGlobalPidData;

typedef struct {
    SHMGlobalARData  rdata;
    SHMGlobalARData  tdata;
    SHMGlobalPidData pdata;
    bool             is_active_inhibitor;  
} SHMGlobalData;


void init_shm();
void close_shm();
void clear_shm();

SHMGlobalData* get_shm_pointer();

#endif